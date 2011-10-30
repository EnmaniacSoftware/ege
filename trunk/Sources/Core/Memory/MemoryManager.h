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

    static void* Malloc(size_t size, const char* fileName, int line);
    static void* Realloc(void* data, size_t size, const char* fileName, int line);
    static void  Free(void* data);
    static void* DoMalloc(size_t size);
    static void* DoRealloc(void* data, size_t size);
    static void  DoFree(void* data);

    static void* MemCpy(void* dst, const void* src, size_t size);
    static void* MemMove(void* dst, const void* src, size_t size);
    static void* MemSet(void* dst, int c, size_t size);

  private:

    bool addAlloc(void* data, size_t size, const char* fileName, int line);
    void removeAlloc(void* data);
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