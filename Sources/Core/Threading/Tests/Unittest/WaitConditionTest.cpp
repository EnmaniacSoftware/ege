#include "TestFramework/Interface/TestBase.h"
#include <EGEDevice.h>
#include <EGEMemory.h>
#include <EGEMutex.h>
#include <EGEWaitCondition.h>
#include <pthread.h>
#include <map>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const s32 KRepetitionsCount = 100;
static const s32 KThreadCount      = 20;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef void* (*threadMain)(void* userData);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class WaitConditionTest : public TestBase
{
  public:

    static const s64 KDefaultStartTimeoutMs = 5000;

  protected:

    /*! Thread main function for increments. */
    static void* WaitAndIncrement(void* userData);
    /*! Thread main function for recursive increments. */
   // static void* RecursiveIncrement(void* userData);

    static void SetUpTestCase();
    static void TearDownTestCase();

  protected:

    virtual void SetUp();
    virtual void TearDown();

    /*! Creates mutex of a given type and condition variable. */
    void createMutexAndCondition(EGEMutex::EType type);

    /*! Creates number of threads executing given function. */
    void createThreads(s32 count, threadMain main);
    /*! Signals threads to start execution. */
    void startThreads();
    /*! Waits until all threads are finished. */
    void waitUntilThreadsFinish();
    /*! Waits until one thread is finished. 
     *  @note When such thread is finished its data is cleaned up from internal list.
     */
    void waitUntilThreadFinishes();

  protected:

    /*! Pool of threads. */
    std::vector<std::pair<pthread_t, bool> > m_threads;
    /*! Mutex under test. */
    PMutex m_mutex;
    /*! Wait condition. */
    PWaitCondition m_condition;
    /*! Counter. */
    s32 m_counter;
    /*! Awaiting threads counter. */
    s32 m_awaitingCounter;
    /*! Flag allowing threads to start execution. */
    bool m_threadStartFlag;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void WaitConditionTest::SetUpTestCase()
{
  EXPECT_TRUE(MemoryManager::Initialize());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void WaitConditionTest::TearDownTestCase()
{
  MemoryManager::Deinitialize();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void WaitConditionTest::SetUp()
{
  m_counter = 0;
  m_awaitingCounter = 0;
  m_threadStartFlag = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void WaitConditionTest::TearDown()
{
  m_condition = NULL;
  m_mutex = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void WaitConditionTest::createMutexAndCondition(EGEMutex::EType type)
{
  EXPECT_TRUE(NULL == m_mutex);
  EXPECT_TRUE(NULL == m_condition);
  m_mutex = ege_new Mutex(type);
  m_condition = ege_new WaitCondition();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* WaitConditionTest::WaitAndIncrement(void* userData)
{
  WaitConditionTest* me = reinterpret_cast<WaitConditionTest*>(userData);

  while ( ! me->m_threadStartFlag)
  {
    // wait
  }

  // lock mutex
  EXPECT_TRUE(me->m_mutex->lock());

  // increase awaiting counter
  ++me->m_awaitingCounter;

  // wait for signal
  EXPECT_TRUE(me->m_condition->wait(me->m_mutex));

  // do some work
  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    me->m_counter++;
  }

  // decrease awaiting counter
  --me->m_awaitingCounter;

  // mark thread as done
  for (std::vector<std::pair<pthread_t, bool> >::iterator it = me->m_threads.begin(); it != me->m_threads.end(); ++it)
  {
    std::pair<pthread_t, bool>& threadInfo = *it;

    if (0 != pthread_equal(threadInfo.first, pthread_self()))
    {
      threadInfo.second = false;
      break;
    }
  }

  // release mutex
  EXPECT_TRUE(me->m_mutex->unlock());

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void WaitConditionTest::createThreads(s32 count, threadMain main)
{
  for (s32 i = 0; i < count; ++i)
  {
    pthread_t thread;
    EXPECT_EQ(0, pthread_create(&thread, NULL, main, this));
  
    m_threads.push_back(std::pair<pthread_t, bool>(thread, true));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void WaitConditionTest::startThreads()
{
  Device::Sleep(1000);
  m_threadStartFlag = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void WaitConditionTest::waitUntilThreadsFinish()
{
  for (std::vector<std::pair<pthread_t, bool> >::const_iterator it = m_threads.begin(); it != m_threads.end(); ++it)
  {
    const pthread_t& thread = it->first;

    void* status;
    EXPECT_EQ(0, pthread_join(thread, &status));
    EXPECT_EQ(0, status);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void WaitConditionTest::waitUntilThreadFinishes()
{
  while (true)
  {
    for (std::vector<std::pair<pthread_t, bool> >::const_iterator it = m_threads.begin(); it != m_threads.end(); ++it)
    {
      const pthread_t& thread = it->first;
      bool running = it->second;

      if ( ! it->second)
      {
        m_threads.erase(it);
        return;
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// TAGE - Enable when EGE #111 is done
TEST_F(WaitConditionTest, DISABLED_WaitOnUnlockedMutex)
{
  WaitCondition condition;

  createMutexAndCondition(EGEMutex::Normal);

  EXPECT_FALSE(condition.wait(m_mutex));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// TAGE - Enable when EGE #111 is done
TEST_F(WaitConditionTest, DISABLED_WaitOnUnlockedRecursiveMutex)
{
  WaitCondition condition;

  createMutexAndCondition(EGEMutex::Recursive);

  EXPECT_FALSE(condition.wait(m_mutex));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// TAGE - Enable when EGE #111 is done
TEST_F(WaitConditionTest, DISABLED_WaitOnNoMutex)
{
  WaitCondition condition;

  EXPECT_FALSE(condition.wait(NULL));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(WaitConditionTest, UID)
{
  WaitCondition condition;
  EXPECT_EQ(EGE_OBJECT_UID_WAIT_CONDITION, condition.uid());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(WaitConditionTest, WakeOne)
{
  createMutexAndCondition(EGEMutex::Normal);
  createThreads(KThreadCount, WaitConditionTest::WaitAndIncrement);
  startThreads();

  // wait until all threads are awaiting
  while (m_awaitingCounter != KThreadCount)
  {
  }

  // wake up threads one by one
  for (s32 i = 0; i < KThreadCount; ++i)
  {    
    EXPECT_EQ(i * KRepetitionsCount, m_counter);

    m_condition->wakeOne();

    // wait for the thread to do its work
    waitUntilThreadFinishes();
  }

  // wait for all threads to finish
  waitUntilThreadsFinish();

  // check if work has been done
  EXPECT_EQ(KThreadCount * KRepetitionsCount, m_counter);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(WaitConditionTest, WakeAll)
{
  createMutexAndCondition(EGEMutex::Normal);
  createThreads(KThreadCount, WaitConditionTest::WaitAndIncrement);
  startThreads();

  // wait until all threads are awaiting
  while (m_awaitingCounter != KThreadCount)
  {
  }

  // wake up all
  // NOTE: mutex used to make sure this thread awaits until last worker thread actually starts awaiting so it does not miss the wakeup signal
  EXPECT_TRUE(m_mutex->lock());
  m_condition->wakeAll();
  EXPECT_TRUE(m_mutex->unlock());

  // wait for all threads to finish
  waitUntilThreadsFinish();

  // check if work has been done
  EXPECT_EQ(KThreadCount * KRepetitionsCount, m_counter);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------