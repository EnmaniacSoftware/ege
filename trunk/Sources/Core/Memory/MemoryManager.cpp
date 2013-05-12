#include "Core/Memory/MemoryManager.h"
#include "EGEDebug.h"
#include "EGEMutex.h"
#include <fstream>
#include <stdarg.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define INTEGRITY_CHECK 1
#define ORDERING 1
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static MemoryManager* l_instance = NULL;
static PMutex l_mutex = NULL;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MemoryManager::MemoryManager() :  m_allocs(NULL), 
                                  m_allocCount(0), 
                                  m_allocUsed(0),
                                  m_bytesAllocated(0)
{
  internalRealloc(1000);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MemoryManager::~MemoryManager()
{
  finalize();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MemoryManager* MemoryManager::GetInstance()
{
  return l_instance;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool MemoryManager::Initialize()
{
  // allocate recursive mutex
  // NOTE: it is allocated outside the manager scope
  l_mutex = ege_new Mutex(NULL, EGEMutex::Recursive);

  // allocate memory manager
  l_instance = new MemoryManager();

  return (NULL != l_mutex) && (NULL != l_instance);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MemoryManager::Deinitialize()
{
  EGE_DELETE(l_instance);
  EGE_DELETE(l_mutex);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* MemoryManager::Malloc(size_t size, const char* fileName, int line)
{
  void* data = MemoryManager::DoMalloc(size);

#if EGE_FEATURE_MEMORY_DEBUG
  if (NULL != data)
  {
    // check if manager is up
    if (NULL != MemoryManager::GetInstance())
    {
      bool result = MemoryManager::GetInstance()->addAlloc(data, size, fileName, line);
      EGE_ASSERT(result);
    }
    else
    {
      // report allocate out of manager scope
      ShowMessage("WARNING: Out of scope allocation %p in %s @ %d (%u bytes)", data, fileName, line, static_cast<u32>(size));
    }
  }
  else
  {
    ShowMessage("WARNING: Could not allocate memory: %u in %s @ %d", static_cast<u32>(size), fileName, line);
  }
#endif // EGE_FEATURE_MEMORY_DEBUG

  return data;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* MemoryManager::Realloc(void* data, size_t size, const char* fileName, int line)
{
  void* newData = NULL;

  // check if first allocation
  if (NULL == data)
  {
    // allocate as normal
    newData = MemoryManager::Malloc(size, fileName, line);
  }
  else
  {
    // reallocate
    newData = MemoryManager::DoRealloc(data, size);
    if (NULL != newData)
    {
      // check if manager is up
      if (NULL != MemoryManager::GetInstance())
      {
        MemoryManager::GetInstance()->doRealloc(data, newData, size);
      }
      else
      {
        // report reallocation out of manager scope
        ShowMessage("WARNING: Out of scope reallocation %p in %s @ %d (%u bytes)", data, fileName, line, static_cast<u32>(size));
      }
    }
  }

#if EGE_FEATURE_MEMORY_DEBUG
  if (NULL == newData)
  {
    ShowMessage("WARNING: Could not reallocate memory: %u in %s @ %d", static_cast<u32>(size), fileName, line);
  }
#endif // EGE_FEATURE_MEMORY_DEBUG

  return newData;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MemoryManager::Free(void* data)
{
  // check if manager is up
  if (NULL != MemoryManager::GetInstance())
  {
    MemoryManager::GetInstance()->removeAlloc(data);
  }
  else
  {
    // report freeing out of manager scope
    ShowMessage("WARNING: Out of scope deallocation %p", data);
  }

  MemoryManager::DoFree(data);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u64 MemoryManager::BytesAllocated()
{
  EGE_ASSERT(NULL != MemoryManager::GetInstance());
  return MemoryManager::GetInstance()->m_bytesAllocated;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool MemoryManager::addAlloc(void* data, size_t size, const char* fileName, int line)
{
  MutexLocker lock(l_mutex);

  // check if reallocation is needed
  if (m_allocCount == m_allocUsed)
  {
    // reallocate internal buffer
    if ( ! internalRealloc(m_allocCount * 2))
    { 
      // error!
      return false;
    }
  }

#if INTEGRITY_CHECK
  for (int i = 0; i < m_allocUsed; ++i)
  {
    SALLOCDATA* allocData = &m_allocs[i];

    if ((reinterpret_cast<u8*>(data) >= reinterpret_cast<u8*>(allocData->data)) && 
        (reinterpret_cast<u8*>(data) < reinterpret_cast<u8*>(allocData->data) + allocData->size))
    {
      EGE_ASSERT("Overlapping!");
    }
  }
#endif // INTEGRITY_CHECK

  int i = m_allocUsed;
#if ORDERING
  for (i = 0; i < m_allocUsed; ++i)
  {
    SALLOCDATA* allocData = &m_allocs[i];

    if (reinterpret_cast<u8*>(data) < reinterpret_cast<u8*>(allocData->data))
    {
      MemoryManager::MemMove(&m_allocs[i + 1], &m_allocs[i], sizeof (SALLOCDATA) * (m_allocUsed - i));
      break;
    }
  }
#endif // ORDERING

  m_allocs[i].count     = 1;
  m_allocs[i].fileName  = fileName;
  m_allocs[i].line      = line;
  m_allocs[i].size      = size;
  m_allocs[i].data      = data;

  m_allocUsed++;
  m_bytesAllocated += size;

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool MemoryManager::doRealloc(void* data, void* newData, size_t size)
{
  MutexLocker lock(l_mutex);

  for (int i = 0; i < m_allocUsed; ++i)
  {
    if (m_allocs[i].data == data)
    {
      m_allocs[i].data = newData;
      m_bytesAllocated -= m_allocs[i].size;
      m_bytesAllocated += size;
      return true;
    }
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MemoryManager::removeAlloc(void* data)
{
  MutexLocker lock(l_mutex);

  for (int i = 0; i < m_allocUsed; ++i)
  {
    if (m_allocs[i].data == data)
    {
      m_bytesAllocated -= m_allocs[i].size;
      MemoryManager::MemMove(&m_allocs[i], &m_allocs[i + 1], sizeof (SALLOCDATA) * (m_allocUsed - i - 1));
      m_allocUsed--;

      return;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MemoryManager::finalize()
{
  MutexLocker lock(l_mutex);

  std::ofstream logFile;
  logFile.open("ege_memoryleak.log");

  // flatten all results from the same place into 1 entry
  for (int i = 0; i < m_allocUsed; ++i)
  {
    SALLOCDATA* allocData1 = &m_allocs[i];

    for (int j = i + 1; j < m_allocUsed; ++j)
    {
      SALLOCDATA* allocData2 = &m_allocs[j];
      
      if ((allocData1->fileName == allocData2->fileName) && (allocData1->line == allocData2->line) && (0 != allocData2->count))
      {
        allocData1->count++;
        allocData1->size += allocData2->size;

        allocData2->count = 0;
      }
    }
  }

  // show all unique entries
  for (int i = 0; i < m_allocUsed; ++i)
  {
    const SALLOCDATA* allocData = &m_allocs[i];
  
    if (0 != allocData->count)
    {
      logFile << "Leak: " << allocData->size << " bytes (" << allocData->count << " times) in " << allocData->fileName;
      logFile << ", line: " << allocData->line << "\n";
    }
  }

  logFile.close();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool MemoryManager::internalRealloc(int newSize)
{
  MutexLocker lock(l_mutex);

  if (newSize <= m_allocCount)
  {
    // do nothing
    return true;
  }

  SALLOCDATA* newAllocs = reinterpret_cast<SALLOCDATA*>(MemoryManager::DoRealloc(m_allocs, sizeof(SALLOCDATA) * newSize));
  if (NULL != newAllocs)
  {
    m_allocs     = newAllocs;
    m_allocCount = newSize;
    return true;
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MemoryManager::ShowMessage(const char* text, ...)
{
  char buffer[256];

	va_list arg;
	va_start(arg, text);
	vsprintf(buffer, text, arg);
	va_end(arg);

  Debug::Print(buffer);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END