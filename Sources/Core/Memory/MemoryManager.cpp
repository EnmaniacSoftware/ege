#include "Core/Memory/MemoryManager.h"
#include "EGEDebug.h"
#include "EGEMutex.h"
#include <list>
#include <fstream>
#include <stdarg.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static MemoryManager* l_instance = NULL;
static PMutex l_mutex;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MemoryManager::MemoryManager() : m_bytesAllocated(0)
{
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
  l_mutex = NULL;
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
      MemoryManager::GetInstance()->addAlloc(data, size, fileName, line);
    }
    else
    {
      // report allocate out of manager scope
      Debug::PrintWithArgs("WARNING: Out of scope allocation %p in %s @ %d (%u bytes)", data, fileName, line, static_cast<u32>(size));
    }
  }
  else
  {
    Debug::PrintWithArgs("WARNING: Could not allocate memory: %u in %s @ %d", static_cast<u32>(size), fileName, line);
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
        Debug::PrintWithArgs("WARNING: Out of scope reallocation %p in %s @ %d (%u bytes)", data, fileName, line, static_cast<u32>(size));
      }
    }
  }

#if EGE_FEATURE_MEMORY_DEBUG
  if (NULL == newData)
  {
    Debug::PrintWithArgs("WARNING: Could not reallocate memory: %u in %s @ %d", static_cast<u32>(size), fileName, line);
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
    Debug::PrintWithArgs("WARNING: Out of scope deallocation %p", data);
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
void MemoryManager::addAlloc(void* data, size_t size, const char* fileName, int line)
{
  MutexLocker lock(l_mutex);

  EGE_ASSERT(m_allocations.end() == m_allocations.find(data));

  // update statistics
  m_bytesAllocated += size;

  // add allocation to pool
  SAllocData allocData;

  allocData.count     = 1;
  allocData.fileName  = fileName;
  allocData.line      = line;
  allocData.size      = size;

  m_allocations.insert(std::pair<void*, SAllocData>(data, allocData));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MemoryManager::doRealloc(void* data, void* newData, size_t size)
{
  MutexLocker lock(l_mutex);

  // locate allocation
  AllocationMap::iterator it = m_allocations.find(data);

  EGE_ASSERT(m_allocations.end() != it);

  // get copy of allocation data
  SAllocData allocData = it->second;

  // remove it from pool
  m_allocations.erase(it);

  // update statics
  m_bytesAllocated -= allocData.size;
  m_bytesAllocated += size;

  // update
  allocData.size = size;

  // add to pool again
  m_allocations.insert(std::pair<void*, SAllocData>(newData, allocData));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MemoryManager::removeAlloc(void* data)
{
  MutexLocker lock(l_mutex);

  // TAGE - for debugging iOS related crashes
  //const u32 ref = l_mutex->referenceCount();
  
  // locate allocation
  AllocationMap::iterator it = m_allocations.find(data);

  EGE_ASSERT(m_allocations.end() != it);

  // get alloc data
  SAllocData& allocData = it->second;

  // update statistics
  m_bytesAllocated -= allocData.size;

  // remove from pool
  m_allocations.erase(it);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MemoryManager::finalize()
{
  MutexLocker lock(l_mutex);

  std::ofstream logFile;
  logFile.open("ege_memoryleak.log");

  std::list<SAllocData> leaks;

  // flatten all results from the same place into 1 entry
  while ( ! m_allocations.empty())
  {
    // take first available allocation out
    SAllocData allocData = m_allocations.begin()->second;

    // remove it from pool
    m_allocations.erase(m_allocations.begin());

    // go thru the rest and locate allocation at the same place
    // NOTE: do not use const_iterator due to STL implementation for Airplay
    for (AllocationMap::iterator it = m_allocations.begin(); it != m_allocations.end(); )
    {
      const SAllocData& currentAllocData = it->second;

      // is the same place ?
      if ((currentAllocData.fileName == allocData.fileName) && (currentAllocData.line == allocData.line))
      {
        // add allocation data
        allocData.size += currentAllocData.size;
        allocData.count++;

        // remove from pool
        m_allocations.erase(it++);
      }
      else
      {
        ++it;
      }
    }

    // check if any leaks
    if (0 < allocData.size)
    {
      // add to pool of leaked allocations
      leaks.push_back(allocData);
    }
  }

  // show all unique entries
  for (std::list<SAllocData>::const_iterator it = leaks.begin(); it != leaks.end(); ++it)
  {
    const SAllocData& allocData = *it;
  
    logFile << "Leak: " << allocData.size << " bytes (" << allocData.count << " times) in " << allocData.fileName;
    logFile << ", line: " << allocData.line << "\n";
  }

  logFile.close();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END