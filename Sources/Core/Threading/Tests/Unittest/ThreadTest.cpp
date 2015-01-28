#include "TestFramework/Interface/TestBase.h"
#include <EGEMemory.h>
#include <EGEThread.h>
#include <EGETimer.h>
#include <EGESignal.h>
#include <windows.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class WorkerThread : public Thread
{
  public:

    WorkerThread() : m_run(false) {}

  public:

    bool didRun() const { return m_run; }

  protected:

    /*! @see Thread::run */
    EGE::s32 run() override { m_run = true; return 0; } 
 
  private:

    /*! Flag indicating that thread has executed. */
    bool m_run;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class LongTaskWorkerThread : public WorkerThread
{
  public:

    LongTaskWorkerThread(s32 durationMs = 2000) : m_durationLeft(durationMs) {}

  private:

    /*! @see WorkerThread::run */
    EGE::s32 run() override 
    { 
      WorkerThread::run();
      
      while ( ! isStopping() && (0 < m_durationLeft))
      {
        Sleep(1);

        --m_durationLeft;
      }

      return 0; 
    }

  private:

    /*! Work duration left. */
    s32 m_durationLeft;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ThreadTest : public TestBase
{
  public:

    static const s64 KDefaultStartTimeoutMs = 5000;

  public slots:

    /*! Slot called when thread starts. */
    void onThreadStarted(const PThread& thread);
    /*! Slot called when thread finishes. */
    void onThreadFinished(const PThread& thread);

  protected:

    static void SetUpTestCase();
    static void TearDownTestCase();

  protected:

    virtual void SetUp();
    virtual void TearDown();

    /*! Sets up worker thread. */
    void setupWorkerThread();
    /*! Sets up worker thread for a long task. */
    void setupLongTaskWorkerThread();

    /*! Waits, until thread has started or given timeout passes.
     *  @param  timeoutMs   Timeout (in ms) after which waiting is terminated.
     */
    void waitUntilThreadStarted(s64 timeoutMs = KDefaultStartTimeoutMs);
    /*! Waits, until thread has finished or given timeout passes.
     *  @param  timeoutMs   Timeout (in ms) after which waiting is terminated.
     */
    void waitUntilThreadFinished(s64 timeoutMs = KDefaultStartTimeoutMs);

  protected:

    bool m_threadStarted;
    bool m_threadFinished;

    WorkerThread* m_thread;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ThreadTest::SetUpTestCase()
{
  EXPECT_TRUE(MemoryManager::Initialize());
  }
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ThreadTest::TearDownTestCase()
{
  MemoryManager::Deinitialize();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ThreadTest::SetUp()
{
  m_thread         = NULL;
  m_threadStarted  = false;
  m_threadFinished = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ThreadTest::TearDown()
{
  EXPECT_TRUE(NULL != m_thread);
  EGE_DELETE(m_thread);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ThreadTest::onThreadStarted(const PThread& thread)
{
  m_threadStarted = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ThreadTest::onThreadFinished(const PThread& thread)
{
  m_threadFinished = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ThreadTest::setupWorkerThread()
{
  EXPECT_TRUE(NULL == m_thread);
  m_thread = ege_new WorkerThread();
  EXPECT_TRUE(NULL != m_thread);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ThreadTest::setupLongTaskWorkerThread()
{
  EXPECT_TRUE(NULL == m_thread);
  m_thread = ege_new LongTaskWorkerThread();
  EXPECT_TRUE(NULL != m_thread);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ThreadTest::waitUntilThreadStarted(s64 timeoutMs)
{
  while ((0 < timeoutMs) && ! m_threadStarted)
  {
    const s64 timestamp = Timer::GetMiliseconds();

    timeoutMs -= Timer::GetMiliseconds() - timestamp;
  }

  EXPECT_TRUE(m_threadStarted);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ThreadTest::waitUntilThreadFinished(s64 timeoutMs)
{
  while ((0 < timeoutMs) && ! m_threadFinished)
  {
    const s64 timestamp = Timer::GetMiliseconds();

    timeoutMs -= Timer::GetMiliseconds() - timestamp;
  }

  EXPECT_TRUE(m_threadFinished);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ThreadTest, Start)
{
  setupWorkerThread();
  ege_connect(m_thread, started, this, ThreadTest::onThreadStarted);

  EXPECT_TRUE(m_thread->start());

  waitUntilThreadStarted();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ThreadTest, Finish)
{
  setupWorkerThread();
  ege_connect(m_thread, finished, this, ThreadTest::onThreadFinished);

  EXPECT_TRUE(m_thread->start());
  waitUntilThreadFinished();

  EXPECT_TRUE(m_thread->didRun());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ThreadTest, StopWhileRunning)
{
  setupLongTaskWorkerThread();
  ege_connect(m_thread, started, this, ThreadTest::onThreadStarted);
  ege_connect(m_thread, finished, this, ThreadTest::onThreadFinished);

  EXPECT_TRUE(m_thread->start());
  waitUntilThreadStarted();

  m_thread->stop();
  waitUntilThreadFinished();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ThreadTest, CancelWhileRunning)
{
  setupLongTaskWorkerThread();
  ege_connect(m_thread, started, this, ThreadTest::onThreadStarted);
  ege_connect(m_thread, finished, this, ThreadTest::onThreadFinished);

  EXPECT_TRUE(m_thread->start());
  waitUntilThreadStarted();

  m_thread->cancel();

  Sleep(100);
  EXPECT_FALSE(m_threadFinished);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ThreadTest, WaitForFinishWhileRunning)
{
  setupLongTaskWorkerThread();

  ege_connect(m_thread, started, this, ThreadTest::onThreadStarted);
  ege_connect(m_thread, finished, this, ThreadTest::onThreadFinished);

  EXPECT_TRUE(m_thread->start());
  waitUntilThreadStarted();

  m_thread->wait();

  EXPECT_TRUE(m_threadFinished);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------