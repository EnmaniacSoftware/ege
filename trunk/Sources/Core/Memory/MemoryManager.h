#ifndef EGE_CORE_MEMORYMANAGER_H
#define EGE_CORE_MEMORYMANAGER_H

#include "EGE.h"

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

    static u64 BytesAllocated();

  private:

    bool addAlloc(void* data, size_t size, const char* fileName, int line);
    bool doRealloc(void* data, void* newData, size_t size);
    void removeAlloc(void* data);
    void finalize();
    bool internalRealloc(int newSize);

  private:

    struct SALLOCDATA
    {
      void* pData;
      size_t size;
      const char* pszFileName;
      s32 iLine;
      s32 iCount;
    };

    SALLOCDATA* m_psAllocs;

    s32 m_allocCount;
    s32 m_allocUsed;
    u64 m_bytesAllocated;

    static MemoryManager* m_instance;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MEMORYMANAGER_H