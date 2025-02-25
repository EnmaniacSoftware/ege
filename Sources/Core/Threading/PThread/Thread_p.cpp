#include "Core/Threading/PThread/Thread_p.h"
#include "Core/Threading/Thread.h"

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
bool ThreadPrivate::isRunning() const
{
  return m_running;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ThreadPrivate::isFinished() const
{
  return m_finished;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ThreadPrivate::wait()
{
  void* status;
  return 0 == pthread_join(m_thread, &status);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ThreadPrivate::cancel()
{
  pthread_cancel(m_thread);
  EGE_MEMSET(&m_thread, 0, sizeof (m_thread));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* Thread::CurrentId()
{
#if EGE_PLATFORM_AIRPLAY || EGE_PLATFORM_IOS
  return reinterpret_cast<void*>(pthread_self());
#else
  return pthread_self().p;
#endif // EGE_PLATFORM_AIRPLAY
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END