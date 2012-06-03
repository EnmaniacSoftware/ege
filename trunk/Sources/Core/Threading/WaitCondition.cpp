#include <EGEWaitCondition.h>
#include <EGEMutex.h>
#include <EGEDebug.h>

#ifdef EGE_THREAD_PTHREAD
#include "Core/Threading/PThread/WaitCondition_p.h"
#endif // EGE_THREAD_PTHREAD

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(WaitCondition)
EGE_DEFINE_DELETE_OPERATORS(WaitCondition)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
WaitCondition::WaitCondition(Application* app) : Object(app, EGE_OBJECT_UID_WAIT_CONDITION)
{
  m_p = ege_new WaitConditionPrivate(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
WaitCondition::~WaitCondition()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool WaitCondition::isValid() const
{
  return (NULL != m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Releases the locked mutex and waits on the wait condition. 
 *  @note The mutex must be initially locked by the calling thread. If mutex is not in a locked state, this function returns immediately. 
 *  @note Raw pointer is used here so Mutex referece counter is not increased as this will block.
 */
bool WaitCondition::wait(Mutex* mutex)
{
  EGE_ASSERT(isValid());
  EGE_ASSERT((NULL != mutex) && mutex->isValid());
  if (m_p)
  {
    return m_p->wait(mutex);
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Wakes one thread waiting on the wait condition. 
 *  @note The thread that is woken up depends on the operating system's scheduling policies, and cannot be controlled or predicted.
 */
void WaitCondition::wakeOne()
{
  EGE_ASSERT(isValid());
  if (m_p)
  {
    m_p->wakeOne();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Wakes all threads waiting on the wait condition. 
 *  @note The order in which the threads are woken up depends on the operating system's scheduling policies and cannot be controlled or predicted.
 */
void WaitCondition::wakeAll()
{
  EGE_ASSERT(isValid());
  if (m_p)
  {
    m_p->wakeAll();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END