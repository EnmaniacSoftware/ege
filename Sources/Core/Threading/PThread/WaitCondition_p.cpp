#include "EGEMutex.h"
#include "Core/Threading/PThread/WaitCondition_p.h"
#include "Core/Threading/PThread/Mutex_p.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(WaitConditionPrivate)
EGE_DEFINE_DELETE_OPERATORS(WaitConditionPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
WaitConditionPrivate::WaitConditionPrivate(WaitCondition* base)
{
  pthread_cond_init(&m_condition, NULL);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
WaitConditionPrivate::~WaitConditionPrivate()
{
  pthread_cond_destroy(&m_condition);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool WaitConditionPrivate::wait(Mutex* mutex)
{
  bool locked = mutex->m_locked.load();
  EGE_ASSERT(locked);

  if ( ! locked)
  {
    // do nothing
    return false;
  }

  // NOTE: pthread_cond_wait releases the mutex
  mutex->m_locked.store(false);

  int result = pthread_cond_wait(&m_condition, &mutex->p_func()->m_mutex);

  // mutex is locked on pthread_cond_wait return
  mutex->m_locked.store(true);

  return 0 == result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void WaitConditionPrivate::wakeOne()
{
  pthread_cond_signal(&m_condition);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void WaitConditionPrivate::wakeAll()
{
  pthread_cond_broadcast(&m_condition);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END