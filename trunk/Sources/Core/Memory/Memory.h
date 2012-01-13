#ifndef EGE_CORE_MEMORY_H
#define EGE_CORE_MEMORY_H

#include <EGETypes.h>
#include "Core/Memory/Object.h"
#include "Core/Memory/SmartPointer.h"
#include "Core/Memory/MemoryManager.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#define EGE_DELETE(ptr) if (ptr) { delete ptr; ptr = NULL; }
#define EGE_DELETE_ARR(ptr) if (ptr) { delete [] ptr; ptr = NULL; }

#ifdef EGE_FEATURE_DEBUG

  #define EGE_REALLOC(data, size) MemoryManager::Realloc(data, size, __FILE__, __LINE__)
  #define EGE_MALLOC(size) MemoryManager::Malloc(size, __FILE__, __LINE__)
  #define EGE_FREE(ptr) if (ptr) { MemoryManager::Free(ptr); ptr = NULL; }

  #define EGE_DECLARE_NEW_OPERATORS void *operator new (size_t size, const char* filename, int line); \
                                    void *operator new [] (size_t size, const char* filename, int line);

  #define EGE_DECLARE_DELETE_OPERATORS void operator delete (void *ptr); \
                                       void operator delete (void *ptr, const char* filename, int line); \
                                       void operator delete [] (void *ptr); \
                                       void operator delete [](void *ptr, const char* filename, int line);

  #define EGE_DEFINE_NEW_OPERATORS(classname) void* classname::operator new(size_t size, const char* filename, int line) \
                                              { void* pData = EGE::MemoryManager::Malloc(size, filename, line); return pData; } \
                                              void * classname::operator new[](size_t size, const char* filename, int line) \
                                              { void* pData = EGE::MemoryManager::Malloc(size, filename, line); return pData; }

  #define EGE_DEFINE_NEW_OPERATORS_INLINE void* operator new(size_t size, const char* filename, int line) \
                                          { void* pData = EGE::MemoryManager::Malloc(size, filename, line); return pData; } \
                                          void * operator new[](size_t size, const char* filename, int line) \
                                          { void* pData = EGE::MemoryManager::Malloc(size, filename, line); return pData; }

  #define EGE_DEFINE_DELETE_OPERATORS(classname) void classname::operator delete (void *ptr) \
                                                 { EGE::MemoryManager::Free(ptr); } \
                                                 void classname::operator delete (void *ptr, const char* /*filename*/, int /*line*/) \
                                                 { EGE::MemoryManager::Free(ptr); } \
                                                 void classname::operator delete [] (void *ptr) \
                                                 { EGE::MemoryManager::Free(ptr); } \
                                                 void classname::operator delete [](void *ptr, const char* /*filename*/, int /*line*/) \
                                                 { EGE::MemoryManager::Free(ptr); }

  #define EGE_DEFINE_DELETE_OPERATORS_INLINE void operator delete (void *ptr) \
                                             { EGE::MemoryManager::Free(ptr); } \
                                             void operator delete (void *ptr, const char* /*filename*/, int /*line*/) \
                                             { EGE::MemoryManager::Free(ptr); } \
                                             void operator delete [] (void *ptr) \
                                             { EGE::MemoryManager::Free(ptr); } \
                                             void operator delete [](void *ptr, const char* /*filename*/, int /*line*/) \
                                             { EGE::MemoryManager::Free(ptr); }

  #define ege_new new (__FILE__, __LINE__)

#else

  #define EGE_REALLOC(data, size) MemoryManager::DoRealloc(data, size)
  #define EGE_MALLOC(size) MemoryManager::DoMalloc(size)
  #define EGE_FREE(ptr) MemoryManager::DoFree(ptr)

  #define EGE_DECLARE_NEW_OPERATORS
  #define EGE_DECLARE_DELETE_OPERATORS
  #define EGE_DEFINE_NEW_OPERATORS(classname)
  #define EGE_DEFINE_DELETE_OPERATORS(classname)

  #define ege_new new

#endif // EGE_FEATURE_DEBUG

#define EGE_MEMCPY(dst, src, size) MemoryManager::MemCpy(dst, src, size)
#define EGE_MEMSET(dst, c, size) MemoryManager::MemSet(dst, c, size)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MEMORY_H