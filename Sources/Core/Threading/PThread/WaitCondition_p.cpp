#include "Core/Threading/PThread/WaitCondition_p.h"
#include "Core/Threading/PThread/Mutex_p.h"
#include "EGEDebug.h"
#include "EGEMutex.h"
#include "EGEThread.h"

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
  if (NULL == mutex)
  {
    return false;
  }

  if (mutex->isRecursive())
  {
    EGE_ASSERT_X( ! mutex->isRecursive(), "WaitCondition cannot work with recursive mutex!");
    return false;
  }

  // check if initially locked
  bool locked = (NULL != mutex->m_owner.load());
  EGE_ASSERT(locked);

  if ( ! locked)
  {
    // do nothing
    return false;
  }

  // NOTE: pthread_cond_wait releases the mutex
  // NOTE: Mutex is locked so it is ok to change internal state
  mutex->m_lockCount = 0;
  mutex->m_owner.store(NULL);

  int result = pthread_cond_wait(&m_condition, &mutex->p_func()->m_mutex);

  // mutex is locked on pthread_cond_wait return
  mutex->m_lockCount = 1;
  mutex->m_owner.store(Thread::CurrentId());

  return (0 == result);
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