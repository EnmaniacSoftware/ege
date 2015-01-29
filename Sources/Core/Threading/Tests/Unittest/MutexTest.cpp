#include "TestFramework/Interface/TestBase.h"
#include <EGEMemory.h>
#include <EGEMutex.h>
#include <pthread.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const s32 KRepetitionsCount = 100;
static const s32 KThreadCount      = 20;
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
    /*! Waits until all threads are finished. */
    void waitUntilThreadsFinish();

  protected:

    /*! Pool of threads. */
    std::vector<pthread_t> m_thread;
    /*! Mutex under test. */
    PMutex m_mutex;
    /*! Counter. */
    s32 m_counter;
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

  for (int i = 0; i < KRepetitionsCount; ++i)
  {
    me->m_mutex->lock();

    me->m_counter++;

    me->m_mutex->unlock();
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* MutexTest::RecursiveIncrement(void* userData)
{
  MutexTest* me = reinterpret_cast<MutexTest*>(userData);

  me->m_mutex->lock();

  me->m_counter++;
  if (KRepetitionsCount > me->m_counter)
  {
    RecursiveIncrement(userData);
  }

  me->m_mutex->unlock();

  return 0;
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
void MutexTest::waitUntilThreadsFinish()
{
  for (std::vector<pthread_t>::const_iterator it = m_thread.begin(); it != m_thread.end(); ++it)
  {
    const pthread_t& thread = *it;

    void* status;
    EXPECT_EQ(0, pthread_join(thread, &status));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MutexTest, LockUnlock)
{
  createMutex(EGEMutex::Normal);
  createThreads(KThreadCount, MutexTest::Increment);
  waitUntilThreadsFinish();

  EXPECT_EQ(KThreadCount * KRepetitionsCount, m_counter);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MutexTest, LockUnlockRecursive)
{
  createMutex(EGEMutex::Recursive);
  createThreads(1, MutexTest::RecursiveIncrement);
  waitUntilThreadsFinish();

  EXPECT_EQ(KRepetitionsCount, m_counter);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------