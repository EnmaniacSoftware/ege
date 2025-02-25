#ifndef EGE_CORE_MEMORYMANAGER_H
#define EGE_CORE_MEMORYMANAGER_H

#include "EGE.h"
#include <map>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class MemoryManager
{
  public:

    MemoryManager();
   ~MemoryManager();

  public:

    /*! Returns object instance. */
    static MemoryManager* GetInstance();
    /*! Returns number of bytes allocated so far. */
    static u64 BytesAllocated();

  public:

    /*! Initializes object.
     *  @return TRUE if object was properly initialized.
     *  @note This should be called from single-threaded environment.
     */
    static bool Initialize();
    /*! Deitializes object.
     *  @note This should be called from single-threaded environment.
     */
    static void Deinitialize();

    /*! Allocates memory with logging.
     *  @param  size      Number of bytes to allocate.
     *  @param  fileName  Name of the file where allocation takes place.
     *  @param  line      Line number in the file where allocation takes place.
     *  @return Pointer to allocated data. NULL if error occured.
     *  @note   This method should only be used when EGE_FEATURE_MEMORY_DEBUG is defined.
     */
    static void* Malloc(size_t size, const char* fileName, int line);
    /*! Rellocates memory with logging.
     *  @param  data      Pointer to previously allocated space which needs to be reallocated.
     *  @param  size      Number of bytes to allocate.
     *  @param  fileName  Name of the file where allocation takes place.
     *  @param  line      Line number in the file where allocation takes place.
     *  @return Pointer to allocated data. NULL if error occured.
     *  @note   This method should only be used when EGE_FEATURE_MEMORY_DEBUG is defined.
     */
    static void* Realloc(void* data, size_t size, const char* fileName, int line);
    /*! Frees up allocated memory with logging.
     *  @param  data  Pointer to previously allocated space which needs to be freed.
     *  @note   This method should only be used when EGE_FEATURE_MEMORY_DEBUG is defined.
     */
    static void  Free(void* data);

    /*! Allocates memory.
     *  @param  size  Number of bytes to allocate.
     *  @return Pointer to allocated data. NULL if error occured.
     */
    static void* DoMalloc(size_t size);
    /*! Rellocates memory.
     *  @param  data  Pointer to previously allocated space which needs to be reallocated.
     *  @param  size  Number of bytes to allocate.
     *  @return Pointer to allocated data. NULL if error occured.
     */
    static void* DoRealloc(void* data, size_t size);
    /*! Frees up allocated memory.
     *  @param  data  Pointer to previously allocated space which needs to be freed.
     */
    static void  DoFree(void* data);

    static void* MemCpy(void* dst, const void* src, size_t size);
    static void* MemMove(void* dst, const void* src, size_t size);
    static void* MemSet(void* dst, int c, size_t size);
    static s32   MemCmp(const void* ptr1, const void* ptr2, size_t size);

  private:

    /*! Logs allocation.
     *  @param  data      Pointer to allocated space.
     *  @param  size      Number of bytes to allocate.
     *  @param  fileName  Name of the file where allocation takes place.
     *  @param  line      Line number in the file where allocation takes place.
     */
    void addAlloc(void* data, size_t size, const char* fileName, int line);
    /*! Logs reallocation.
     *  @param  data    Pointer to previously allocated space.
     *  @param  newData Pointer to reallocated space.
     *  @param  size    Number of bytes to allocate.
     */
    void doRealloc(void* data, void* newData, size_t size);
    /*! Logs deallocation.
     *  @param  data  Pointer to deallocated space.
     */
    void removeAlloc(void* data);
    /*! Finalizes current state of allocations.
     *  @note This function generates the log file with all unfreed allocations.
     */
    void finalize();

  private:

    /*! Data structure with allocation information. */
    struct SAllocData
    {
      size_t size;                  /*!< Size of allocated space (in bytes). */
      const char* fileName;         /*!< File name where allocation took place. */
      s32 line;                     /*!< Line within the file where allocation took place. */
      s32 count;                    /*!< Number of same allocation which took place so far. */
    };

    /*! Allocation map sorted by allocated pointers. */
    typedef std::map<void*, SAllocData> AllocationMap;
   
  private:

    /*! Allocations pool. */
    AllocationMap m_allocations;
    /*! Cached value of total number of bytes allocated so far. */
    u64 m_bytesAllocated;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MEMORYMANAGER_H