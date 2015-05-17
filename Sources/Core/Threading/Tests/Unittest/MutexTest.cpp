#include "TestFramework/Interface/TestBase.h"
#include <EGEDevice.h>
#include <EGEMemory.h>
#include <EGEMutex.h>
#include <pthread.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const s32 KRepetitionsCount          = 100000;
static const s32 KRecursiveRepetitionsCount = 1000;
static const s32 KThreadCount               = 20;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef void* (*threadMain)(void* userData);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class MutexTest : public TestBase
{
  public:

    static const s64 KDefaultStartTimeoutMs = 5000;

  protected:

    /*! Thread main function for increments. */
    static void* Increment(void* userData);
    /*! Thread main function for recursive increments. */
    static void* RecursiveIncrement(void* userData);

    static void SetUpTestCase();
    static void TearDownTestCase();

  protected:

    virtual void SetUp();
    virtual void TearDown();

    /*! Creates mutex of a given type. */
    void createMutex(EGEMutex::EType type);

    /*! Creates number of threads executing given function. */
    void createThreads(s32 count, threadMain main);
    /*! Signals threads to start execution. */
    void startThreads();
    /*! Waits until all threads are finished. */
    void waitUntilThreadsFinish();

  protected:

    /*! Pool of threads. */
    std::vector<pthread_t> m_thread;
    /*! Mutex under test. */
    PMutex m_mutex;
    /*! Counter. */
    s32 m_counter;
    /*! Flag allowing threads to start execution. */
    bool m_threadStartFlag;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MutexTest::SetUpTestCase()
{
  EXPECT_TRUE(MemoryManager::Initialize());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MutexTest::TearDownTestCase()
{
  MemoryManager::Deinitialize();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MutexTest::SetUp()
{
  m_counter = 0;
  m_threadStartFlag = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MutexTest::TearDown()
{
  EXPECT_TRUE(NULL != m_mutex);
  m_mutex = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MutexTest::createMutex(EGEMutex::EType type)
{
  EXPECT_TRUE(NULL == m_mutex);
  m_mutex = ege_new Mutex(type);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* MutexTest::Increment(void* userData)
{
  MutexTest* me = reinterpret_cast<MutexTest*>(userData);

  while ( ! me->m_threadStartFlag)
  {
    // wait
  }

  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    EXPECT_TRUE(me->m_mutex->lock());

    me->m_counter++;

    EXPECT_TRUE(me->m_mutex->unlock());
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* MutexTest::RecursiveIncrement(void* userData)
{
  MutexTest* me = reinterpret_cast<MutexTest*>(userData);

  void* value = 0;

  while ( ! me->m_threadStartFlag)
  {
    // wait
  }

  // acquire lock
  EXPECT_TRUE(me->m_mutex->lock());

  me->m_counter++;
  if (KRecursiveRepetitionsCount > me->m_counter)
  {
    value = RecursiveIncrement(userData);
  }

  // release lock
  EXPECT_TRUE(me->m_mutex->unlock());

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MutexTest::createThreads(s32 count, threadMain main)
{
  for (s32 i = 0; i < count; ++i)
  {
    pthread_t thread;
    EXPECT_EQ(0, pthread_create(&thread, NULL, main, this));
  
    m_thread.push_back(thread);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MutexTest::startThreads()
{
  Device::Sleep(1000);
  m_threadStartFlag = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MutexTest::waitUntilThreadsFinish()
{
  for (std::vector<pthread_t>::const_iterator it = m_thread.begin(); it != m_thread.end(); ++it)
  {
    const pthread_t& thread = *it;

    void* status;
    EXPECT_EQ(0, pthread_join(thread, &status));
    EXPECT_EQ(0, status);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MutexTest, LockUnlock)
{
  createMutex(EGEMutex::Normal);
  createThreads(KThreadCount, MutexTest::Increment);
  startThreads();
  waitUntilThreadsFinish();

  EXPECT_EQ(KThreadCount * KRepetitionsCount, m_counter);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MutexTest, LockUnlock_RecursiveSingleThread)
{
  createMutex(EGEMutex::Recursive);
  createThreads(1, MutexTest::RecursiveIncrement);
  startThreads();
  waitUntilThreadsFinish();

  EXPECT_EQ(KRecursiveRepetitionsCount, m_counter);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MutexTest, LockUnlock_RecursiveMultiThread)
{
  createMutex(EGEMutex::Recursive);
  createThreads(KThreadCount, MutexTest::RecursiveIncrement);
  startThreads();
  waitUntilThreadsFinish();

  // NOTE: only the first thread will be able to process requested amount of times. Rest will simply increment once.
  EXPECT_EQ(KRecursiveRepetitionsCount + (KThreadCount - 1), m_counter);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------