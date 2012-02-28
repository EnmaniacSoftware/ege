#include "Core/Thread/PThread/Thread_p.h"
#include "Core/Thread/Thread.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ThreadPrivate)
EGE_DEFINE_DELETE_OPERATORS(ThreadPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ThreadPrivate::ThreadPrivate(Thread* base) : m_d(base),
                                             m_running(false),
                                             m_finished(false)
{
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

  // set flags
  me->m_running = true;

  // emit
  emit me->d_func()->started(me->d_func());

  // start thread function execution
  s32 result = me->d_func()->run();
  
  // check if was requested to stop
  if (me->d_func()->isStopping())
  {
    // get requested exit code
    result = me->d_func()->m_exitCode;
  }

  // set flags
  me->m_running            = false;
  me->m_finished           = true;
  me->d_func()->m_stopping = false;

  // emit
  emit me->d_func()->finished(me->d_func());

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
