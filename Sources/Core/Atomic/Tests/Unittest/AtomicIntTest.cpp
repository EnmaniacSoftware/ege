#include "TestFramework/Interface/TestBase.h"
#include <EGEAtomicInt.h>
#include <pthread.h>
#include <vector>
#include <Windows.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const s32 KRepetitionsCount = 100;
static const s32 KThreadCount      = 20;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef void* (*threadMain)(void* userData);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AtomicIntTest : public TestBase
{
  protected:

    /*! Thread main function for increments. */
    static void* Increment(void* userData);
    /*! Thread main function for decrements. */
    static void* Decrement(void* userData);

    /*! Methods sleeping for a given interval. 
     *  @param  miliseconds Sleep interval.
     */
    static void OsSleep(u32 miliseconds);

  protected:

    virtual void SetUp();
    virtual void TearDown();

    void createThreads(s32 count, threadMain main);
    void startThreads();
    void waitUntilThreadsFinish();

  protected:

    /*! Pool of threads. */
    std::vector<pthread_t> m_thread;
    /*! Value to be modifed. */
    AtomicInt m_value;
    /*! Global thread activation flag. */
    bool m_start;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* AtomicIntTest::Increment(void* userData)
{
  AtomicIntTest* me = reinterpret_cast<AtomicIntTest*>(userData);

  while ( ! me->m_start)
  {
    AtomicIntTest::OsSleep(1);
  }

  for (s32 i = 0; i < KRepetitionsCount; ++i)
  {
    me->m_value.increment();
  }

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* AtomicIntTest::Decrement(void* userData)
{
  AtomicIntTest* me = reinterpret_cast<AtomicIntTest*>(userData);

  while ( ! me->m_start)
  {
    AtomicIntTest::OsSleep(1);
  }

  for (s32 i = 0; i < KRepetitionsCount; ++i)
  {
    me->m_value.decrement();
  }

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AtomicIntTest::OsSleep(u32 miliseconds)
{
  Sleep(miliseconds);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AtomicIntTest::SetUp()
{
  m_value = AtomicInt(0);
  m_start = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AtomicIntTest::TearDown()
{
  m_thread.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AtomicIntTest::createThreads(s32 count, threadMain main)
{
  for (s32 i = 0; i < count; ++i)
  {
    pthread_t thread;
    EXPECT_EQ(0, pthread_create(&thread, NULL, main, this));
  
    m_thread.push_back(thread);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AtomicIntTest::startThreads()
{
  m_start = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AtomicIntTest::waitUntilThreadsFinish()
{
  for (std::vector<pthread_t>::const_iterator it = m_thread.begin(); it != m_thread.end(); ++it)
  {
    const pthread_t& thread = *it;

    void* status;
    EXPECT_EQ(0, pthread_join(thread, &status));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AtomicIntTest, IncrementAndLoad)
{
  createThreads(KThreadCount, AtomicIntTest::Increment);
  startThreads();
  waitUntilThreadsFinish();

  EXPECT_EQ(KThreadCount * KRepetitionsCount, m_value.load());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AtomicIntTest, DecrementAndLoad)
{
  createThreads(KThreadCount, AtomicIntTest::Decrement);
  startThreads();
  waitUntilThreadsFinish();

  EXPECT_EQ(-KThreadCount * KRepetitionsCount, m_value.load());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------