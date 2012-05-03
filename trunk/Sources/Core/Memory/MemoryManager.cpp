#include "Core/Memory/MemoryManager.h"
#include <EGEDebug.h>
#include <fstream>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

MemoryManager* MemoryManager::m_instance = NULL;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MemoryManager::MemoryManager() : m_psAllocs(NULL), m_iAllocCount(0), m_iAllocUsed(0)
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
  if (pData)
  {
    bool result = MemoryManager::GetInstance()->addAlloc(pData, size, pszFileName, iLine);
    EGE_ASSERT(result);
  }

  return pData;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* MemoryManager::Realloc(void* pData, size_t size, const char* pszFileName, int iLine)
{
  pData = MemoryManager::DoRealloc(pData, size);

  return pData;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MemoryManager::Free(void* pData)
{
  MemoryManager::GetInstance()->removeAlloc(pData);
  MemoryManager::DoFree(pData);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool MemoryManager::addAlloc(void* pData, size_t size, const char* pszFileName, int iLine)
{
  // check if reallocation is needed
  if (m_iAllocCount == m_iAllocUsed)
  {
    if (!internalRealloc(m_iAllocCount * 2))
    {
      return false;
    }
  }

  m_psAllocs[m_iAllocUsed].iCount      = 1;
  m_psAllocs[m_iAllocUsed].pszFileName = pszFileName;
  m_psAllocs[m_iAllocUsed].iLine       = iLine;
  m_psAllocs[m_iAllocUsed].size        = size;
  m_psAllocs[m_iAllocUsed].pData       = pData;

  m_iAllocUsed++;

  //wchar_t buffer[128];
  //wsprintf(buffer, L"allocating: %p\n", pData);
  //OutputDebugString(buffer);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MemoryManager::removeAlloc(void* pData)
{
  for (int i = 0; i < m_iAllocUsed; ++i)
  {
    if (m_psAllocs[i].pData == pData)
    {
      MemoryManager::MemMove(&m_psAllocs[i], &m_psAllocs[i + 1], sizeof (SALLOCDATA) * (m_iAllocUsed - i - 1));
      m_iAllocUsed--;
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
  for (int i = 0; i < m_iAllocUsed; ++i)
  {
    SALLOCDATA* psData = &m_psAllocs[i];

    for (int j = i + 1; j < m_iAllocUsed; ++j)
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
  for (int i = 0; i < m_iAllocUsed; ++i)
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
  if (newSize <= m_iAllocCount)
  {
    // do nothing
    return true;
  }

  SALLOCDATA* psNewAllocs = (SALLOCDATA*) MemoryManager::DoRealloc(m_psAllocs, sizeof(SALLOCDATA) * newSize);
  if (psNewAllocs)
  {
    m_psAllocs = psNewAllocs;
    m_iAllocCount = newSize;
    return true;
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
