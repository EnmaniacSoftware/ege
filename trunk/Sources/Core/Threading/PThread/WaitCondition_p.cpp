#include <EGEMutex.h>
#include "Core/Threading/PThread/WaitCondition_p.h"
#include "Core/Threading/PThread/Mutex_p.h"
#include <EGEDebug.h>

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
/*! Releases the locked mutex and waits on the wait condition. 
 *  @note The mutex must be initially locked by the calling thread. If mutex is not in a locked state, this function returns immediately. 
 *  @note Raw pointer is used here so Mutex referece counter is not increased as this will block.
 */
bool WaitConditionPrivate::wait(Mutex* mutex)
{
  EGE_ASSERT(mutex->m_locked);

  if ( ! mutex->m_locked)
  {
    // do nothing
    return false;
  }

  // NOTE: pthread_cond_wait releases the mutex
  mutex->m_locked = false;

  int result = pthread_cond_wait(&m_condition, &mutex->p_func()->m_mutex);

  // mutex is locked on pthread_cond_wait return
  mutex->m_locked = true;

  return 0 == result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Wakes one thread waiting on the wait condition. 
 *  @note The thread that is woken up depends on the operating system's scheduling policies, and cannot be controlled or predicted.
 */
void WaitConditionPrivate::wakeOne()
{
  pthread_cond_signal(&m_condition);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Wakes all threads waiting on the wait condition. 
 *  @note The order in which the threads are woken up depends on the operating system's scheduling policies and cannot be controlled or predicted.
 */
void WaitConditionPrivate::wakeAll()
{
  pthread_cond_broadcast(&m_condition);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END