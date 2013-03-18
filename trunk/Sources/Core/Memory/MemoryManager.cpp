#include "Core/Memory/MemoryManager.h"
#include "EGEDebug.h"
#include <fstream>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define INTEGRITY_CHECK 0
#define ORDERING 0
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MemoryManager* MemoryManager::m_instance = NULL;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MemoryManager::MemoryManager() :  m_psAllocs(NULL), 
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
  if (NULL == MemoryManager::m_instance)
  {
    MemoryManager::m_instance = new MemoryManager();
  }

  return MemoryManager::m_instance;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MemoryManager::Deinit()
{
  if (MemoryManager::m_instance)
  {
    delete MemoryManager::m_instance;
    MemoryManager::m_instance = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* MemoryManager::Malloc(size_t size, const char* pszFileName, int iLine)
{
  void* pData = MemoryManager::DoMalloc(size);
  if (NULL != pData)
  {
    bool result = MemoryManager::GetInstance()->addAlloc(pData, size, pszFileName, iLine);
    EGE_ASSERT(result);
  }
#ifdef EGE_FEATURE_MEMORY_DEBUG
  else
  {
    egeWarning() << "Could not allocate memory:" << size << pszFileName << iLine;
  }
#endif // EGE_FEATURE_MEMORY_DEBUG

  return pData;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* MemoryManager::Realloc(void* pData, size_t size, const char* pszFileName, int iLine)
{
  void* pNewData = NULL;

  // check if first allocation
  if (NULL == pData)
  {
    // allocate as normal
    pNewData = MemoryManager::Malloc(size, pszFileName, iLine);
  }
  else
  {
    // reallocate
    pNewData = MemoryManager::DoRealloc(pData, size);
    if (NULL != pNewData)
    {
      MemoryManager::GetInstance()->doRealloc(pData, pNewData, size);
    }
  }

#ifdef EGE_FEATURE_MEMORY_DEBUG
  if (NULL == pNewData)
  {
    egeWarning() << "Could not reallocate memory:" << size << pszFileName << iLine;
  }
#endif // EGE_FEATURE_MEMORY_DEBUG

  return pNewData;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MemoryManager::Free(void* pData)
{
  MemoryManager::GetInstance()->removeAlloc(pData);
  MemoryManager::DoFree(pData);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u64 MemoryManager::BytesAllocated()
{
  return MemoryManager::GetInstance()->m_bytesAllocated;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool MemoryManager::addAlloc(void* pData, size_t size, const char* pszFileName, int iLine)
{
  // check if reallocation is needed
  if (m_allocCount == m_allocUsed)
  {
    if (!internalRealloc(m_allocCount * 2))
    {
      return false;
    }
  }

#if INTEGRITY_CHECK
  for (int i = 0; i < m_allocUsed; ++i)
  {
    SALLOCDATA* psData = &m_psAllocs[i];

    if (reinterpret_cast<u8*>(pData) >= reinterpret_cast<u8*>(psData->pData) && 
        reinterpret_cast<u8*>(pData) < reinterpret_cast<u8*>(psData->pData) + psData->size)
    {
      EGE_ASSERT("Overlapping!");
    }
  }
#endif // INTEGRITY_CHECK

  int i = m_allocUsed;
#if ORDERING
  for (i = 0; i < m_allocUsed; ++i)
  {
    SALLOCDATA* psData = &m_psAllocs[i];

    if (reinterpret_cast<u8*>(pData) < reinterpret_cast<u8*>(psData->pData))
    {
      MemoryManager::MemMove(&m_psAllocs[i + 1], &m_psAllocs[i], sizeof (SALLOCDATA) * (m_allocUsed - i));
      break;
    }
  }
#endif // ORDERING

  m_psAllocs[i].iCount      = 1;
  m_psAllocs[i].pszFileName = pszFileName;
  m_psAllocs[i].iLine       = iLine;
  m_psAllocs[i].size        = size;
  m_psAllocs[i].pData       = pData;

  m_allocUsed++;

  m_bytesAllocated += size;

  //wchar_t buffer[128];
  //wsprintf(buffer, L"allocating: %p\n", pData);
  //OutputDebugString(buffer);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool MemoryManager::doRealloc(void* pData, void* pNewData, size_t size)
{
  for (int i = 0; i < m_allocUsed; ++i)
  {
    if (m_psAllocs[i].pData == pData)
    {
      m_psAllocs[i].pData = pNewData;
      m_bytesAllocated -= m_psAllocs[i].size;
      m_bytesAllocated += size;
      return true;
    }
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MemoryManager::removeAlloc(void* pData)
{
  for (int i = 0; i < m_allocUsed; ++i)
  {
    if (m_psAllocs[i].pData == pData)
    {
      m_bytesAllocated -= m_psAllocs[i].size;
      MemoryManager::MemMove(&m_psAllocs[i], &m_psAllocs[i + 1], sizeof (SALLOCDATA) * (m_allocUsed - i - 1));
      m_allocUsed--;

      return;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MemoryManager::finalize()
{
  std::ofstream logFile;
  logFile.open("ege_memoryleak.log");

  // flatten all results from the same place into 1 entry
  for (int i = 0; i < m_allocUsed; ++i)
  {
    SALLOCDATA* psData = &m_psAllocs[i];

    for (int j = i + 1; j < m_allocUsed; ++j)
    {
      SALLOCDATA* psData2 = &m_psAllocs[j];
      
      if (psData->pszFileName == psData2->pszFileName && psData->iLine == psData2->iLine && (0 != psData2->iCount))
      {
        psData->iCount++;
        psData->size += psData2->size;

        psData2->iCount = 0;
      }
    }
  }

  // show all unique entries
  for (int i = 0; i < m_allocUsed; ++i)
  {
    const SALLOCDATA* psData = &m_psAllocs[i];
  
    if (0 != psData->iCount)
    {
      logFile << "Leak: " << psData->size << " bytes (" << psData->iCount << " times) in " << psData->pszFileName;
      logFile << ", line: " << psData->iLine << "\n";
    }
  }

  logFile.close();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool MemoryManager::internalRealloc(int newSize)
{
  if (newSize <= m_allocCount)
  {
    // do nothing
    return true;
  }

  SALLOCDATA* psNewAllocs = (SALLOCDATA*) MemoryManager::DoRealloc(m_psAllocs, sizeof(SALLOCDATA) * newSize);
  if (psNewAllocs)
  {
    m_psAllocs = psNewAllocs;
    m_allocCount = newSize;
    return true;
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END