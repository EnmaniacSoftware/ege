#include "Core/Threading/PThread/Thread_p.h"
#include "Core/Threading/Thread.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ThreadPrivate)
EGE_DEFINE_DELETE_OPERATORS(ThreadPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ThreadPrivate::ThreadPrivate(Thread* base) 
: m_d(base)
, m_cancelled(false)
, m_running(false)
, m_finished(false)
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
  
  // disable cancelling
  // NOTE: if cancel is requested in the meantime it will happen once re-enabled and first opportunity for test arises
  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

  // setup clean up function
  pthread_cleanup_push(ThreadPrivate::ThreadFinalize, me);

  // set flags
  me->m_running.store(true);

  // emit
  emit base->started(*base);
  
  // check if still to be run
  if ( ! base->m_stopping.load())
  {
    // re-enable cancelling and check if it was requested in the meantime
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_testcancel();

    // start thread function execution
    s32 result = base->run();

    // check if thread hasnt been manually stopped
    if ( ! base->isStopping())
    {
      // set exit code according to thread method
      base->m_exitCode = result;
    }
  }

  // finalize
  pthread_cleanup_pop(1);

  return reinterpret_cast<void*>(base->m_exitCode);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ThreadPrivate::ThreadFinalize(ThreadPrivate* thread)
{
  Thread* base = thread->d_func();

  // set flags
  thread->m_running.store(false);
  thread->m_finished.store(true);
  base->m_stopping.store(false);

  // emit
  emit base->finished(*base);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ThreadPrivate::start()
{
  pthread_attr_t attr;

  // initialize and set thread detached attribute
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  // set flags
  m_finished.store(false);
  
  // start thread
  return 0 == pthread_create(&m_thread, &attr, &ThreadPrivate::ThreadFunc, this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ThreadPrivate::isRunning() const
{
  return m_running.load();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ThreadPrivate::isFinished() const
{
  return m_finished.load();
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
  if (0 == pthread_cancel(m_thread))
  {
    m_cancelled.store(true);
  }

  //EGE_MEMSET(&m_thread, 0, sizeof (m_thread));
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