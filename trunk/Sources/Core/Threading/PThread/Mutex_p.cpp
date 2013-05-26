#include "Core/Threading/PThread/Mutex_p.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(MutexPrivate)
EGE_DEFINE_DELETE_OPERATORS(MutexPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MutexPrivate::MutexPrivate(Mutex* base, EGEMutex::EType type)
{
  pthread_mutexattr_t attributes;

  // initialize mutex attributes
  pthread_mutexattr_init(&attributes);

  // convert type
  int attrValue = 0;
  switch (type)
  {
    case EGEMutex::Normal:    attrValue = PTHREAD_MUTEX_NORMAL; break;
    case EGEMutex::Recursive: attrValue = PTHREAD_MUTEX_RECURSIVE; break;

    default:

      EGE_ASSERT(false && "Unknown mutex type");
      break;
  }

  pthread_mutexattr_settype(&attributes, attrValue);

  // initialize mutex
  int result = pthread_mutex_init(&m_mutex, &attributes);
  EGE_ASSERT(0 == result);
  EGE_ASSERT(0 != m_mutex);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MutexPrivate::~MutexPrivate()
{
  pthread_mutex_destroy(&m_mutex);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool MutexPrivate::lock()
{
  return 0 == pthread_mutex_lock(&m_mutex);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool MutexPrivate::unlock()
{
  return 0 == pthread_mutex_unlock(&m_mutex);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END