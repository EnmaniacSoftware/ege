#include <EGEThread.h>
#include <EGEDebug.h>

#ifdef EGE_THREAD_PTHREAD
#include "Core/Threading/PThread/Thread_p.h"
#endif // EGE_THREAD_PTHREAD

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Thread)
EGE_DEFINE_DELETE_OPERATORS(Thread)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Thread::Thread(Application* app) : Object(app, EGE_OBJECT_UID_THREAD),
                                   m_stopping(false),
                                   m_exitCode(0)
{
  m_p = ege_new ThreadPrivate(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Thread::~Thread()
{
  stop(0);
  wait();

  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool Thread::isValid() const
{
  return (NULL != m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Starts thread. */
bool Thread::start()
{
  EGE_ASSERT(isValid());

  // reset data
  m_stopping = false;
  m_exitCode = 0;

  if (m_p)
  {
    // start
    bool result = m_p->start();
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if thread is currently running. */
bool Thread::isRunning() const
{
  EGE_ASSERT(isValid());
  if (m_p)
  {
    return m_p->isRunning();
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if thread has finished its work. */
bool Thread::isFinished() const
{
  EGE_ASSERT(isValid());
  if (m_p)
  {
    return m_p->isFinished();
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Stops thread. */
void Thread::stop(s32 exitCode)
{
  EGE_ASSERT(isValid());
  if (m_p && m_p->isRunning())
  {
    m_stopping = true;
    m_exitCode = exitCode;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if thread has been requested to terminate. */
bool Thread::isStopping() const
{
  return m_stopping;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Waits for thread to be finished. */
bool Thread::wait()
{
  EGE_ASSERT(isValid());
  if (m_p && m_p->isRunning())
  {
    return m_p->wait();
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Cancels tread. 
 *  @note This forcilbly stops the thread. Use with caution.
 */
void Thread::cancel()
{
  EGE_ASSERT(isValid());
  if (m_p && m_p->isRunning())
  {
    m_p->cancel();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END