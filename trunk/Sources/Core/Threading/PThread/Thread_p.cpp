#include "Core/Threading/PThread/Thread_p.h"
#include "Core/Threading/Thread.h"
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ThreadPrivate)
EGE_DEFINE_DELETE_OPERATORS(ThreadPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ThreadPrivate::ThreadPrivate(Thread* base) : m_d(base),
                                             m_running(false),
                                             m_finished(false)
{
  EGE_MEMSET(&m_thread, 0, sizeof (m_thread));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ThreadPrivate::~ThreadPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Thread function. */
void* ThreadPrivate::ThreadFunc(void* userData)
{
  ThreadPrivate* me = reinterpret_cast<ThreadPrivate*>(userData);
  
  Thread* base = me->d_func();

  // set flags
  me->m_running = true;

  // emit
  emit base->started(base);

  // start thread function execution
  s32 result = base->run();
  
  // check if was requested to stop
  if (base->isStopping())
  {
    // get requested exit code
    result = base->m_exitCode;
  }

  // set flags
  me->m_running    = false;
  me->m_finished   = true;
  base->m_stopping = false;

  // emit
  emit base->finished(base);

  // clean up
  pthread_exit(reinterpret_cast<void*>(result));

  return reinterpret_cast<void*>(result);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Starts thread. */
bool ThreadPrivate::start()
{
  pthread_attr_t attr;

  // initialize and set thread detached attribute
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  // set flags
  m_finished = false;
  
  // start thread
  return 0 == pthread_create(&m_thread, &attr, &ThreadPrivate::ThreadFunc, this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if thread is currently running. */
bool ThreadPrivate::isRunning() const
{
  return m_running;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if thread has finished its work. */
bool ThreadPrivate::isFinished() const
{
  return m_finished;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Waits for thread to be finished. */
bool ThreadPrivate::wait()
{
  void* status;
  return 0 == pthread_join(m_thread, &status);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Cancels tread. 
 *  @note This forcilbly stops the thread. Use with caution.
 */
void ThreadPrivate::cancel()
{
  int result = pthread_cancel(m_thread);
  EGE_MEMSET(&m_thread, 0, sizeof (m_thread));
  egeDebug() << "Canceling:" << this << "result code:" << result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END