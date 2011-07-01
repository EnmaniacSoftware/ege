#ifndef EGE_CORE_MEMORYMANAGER_H
#define EGE_CORE_MEMORYMANAGER_H

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "Core/Platform.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class MemoryManager
{
  public:

    MemoryManager();
   ~MemoryManager();

    static MemoryManager* GetInstance();
    static void Deinit();

    static void* Malloc(size_t size, const char* pszFileName, int iLine);
    static void* Realloc(void* pData, size_t size, const char* pszFileName, int iLine);
    static void  Free(void* pData);
    static void* DoMalloc(size_t size);
    static void* DoRealloc(void* pData, size_t size);
    static void  DoFree(void* pData);

    static void* MemCpy(void* pDst, const void* pSrc, size_t size);
    static void* MemMove(void* pDst, const void* pSrc, size_t size);

  private:

    bool addAlloc(void* pData, size_t size, const char* pszFileName, int iLine);
    void removeAlloc(void* pData);
    void finalize();
    bool internalRealloc(int newSize);

  private:

    struct SALLOCDATA
    {
      void* pData;
      size_t size;
      const char* pszFileName;
      int iLine;
      int iCount;
    };

    SALLOCDATA* m_psAllocs;

    int m_iAllocCount;
    int m_iAllocUsed;

    static MemoryManager* m_instance;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MEMORYMANAGER_H