#include "TestFramework/Interface/TestBase.h"
#include <EGEAtomic.h>
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
class AtomicOperationsTest : public TestBase
{
  protected:

    /*! Thread main function for increments. */
    static void* Increment(void* userData);
    /*! Thread main function for decrements. */
    static void* Decrement(void* userData);
    /*! Thread main function for compares and sets. */
    static void* CompareAndSet(void* userData);

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
    volatile s32 m_value;
    /*! Global thread activation flag. */
    volatile s32 m_start;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* AtomicOperationsTest::Increment(void* userData)
{
  AtomicOperationsTest* me = reinterpret_cast<AtomicOperationsTest*>(userData);

  while (0 == me->m_start)
  {
    AtomicOperationsTest::OsSleep(1);
  }

  for (s32 i = 0; i < KRepetitionsCount; ++i)
  {
    egeAtomicIncrement(me->m_value);
  }

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* AtomicOperationsTest::Decrement(void* userData)
{
  AtomicOperationsTest* me = reinterpret_cast<AtomicOperationsTest*>(userData);

  while (0 == me->m_start)
  {
    AtomicOperationsTest::OsSleep(1);
  }

  for (s32 i = 0; i < KRepetitionsCount; ++i)
  {
    egeAtomicDecrement(me->m_value);
  }

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* AtomicOperationsTest::CompareAndSet(void* userData)
{
  AtomicOperationsTest* me = reinterpret_cast<AtomicOperationsTest*>(userData);

  for (s32 i = 0; i < KRepetitionsCount; ++i)
  {
    while (0 != egeAtomicCompareAndSet(me->m_start, 0, 1))
    {
      // wait
      AtomicOperationsTest::OsSleep(1);
    }

    // NOTE: all code from here tukk another egeAtomicCompareAndSet is a kind of critical section, no other thread should be able to share this with some other
    //printf("ENTERING\n");

    ++me->m_value;

   // printf("LEVING\n");

    EXPECT_EQ(1, egeAtomicCompareAndSet(me->m_start, 1, 0));
  }

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AtomicOperationsTest::SetUp()
{
  m_value = 0;
  m_start = 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AtomicOperationsTest::TearDown()
{
  m_thread.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AtomicOperationsTest::OsSleep(u32 miliseconds)
{
  Sleep(miliseconds);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AtomicOperationsTest::createThreads(s32 count, threadMain main)
{
  for (s32 i = 0; i < count; ++i)
  {
    pthread_t thread;
    EXPECT_EQ(0, pthread_create(&thread, NULL, main, this));
  
    m_thread.push_back(thread);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AtomicOperationsTest::startThreads()
{
  m_start = 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AtomicOperationsTest::waitUntilThreadsFinish()
{
  for (std::vector<pthread_t>::const_iterator it = m_thread.begin(); it != m_thread.end(); ++it)
  {
    const pthread_t& thread = *it;

    void* status;
    EXPECT_EQ(0, pthread_join(thread, &status));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AtomicOperationsTest, Increment)
{
  createThreads(KThreadCount, AtomicOperationsTest::Increment);
  startThreads();
  waitUntilThreadsFinish();

  EXPECT_EQ(KThreadCount * KRepetitionsCount, m_value);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AtomicOperationsTest, Decrement)
{
  createThreads(KThreadCount, AtomicOperationsTest::Decrement);
  startThreads();
  waitUntilThreadsFinish();

  EXPECT_EQ(-KThreadCount * KRepetitionsCount, m_value);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AtomicOperationsTest, CompareAndSet)
{
  //EXPECT_EQ(0, egeAtomicCompareAndSet(m_start, 0, 1));
  //EXPECT_EQ(1, egeAtomicCompareAndSet(m_start, 0, 1));
  //EXPECT_EQ(1, egeAtomicCompareAndSet(m_start, 1, 0));
  //EXPECT_EQ(0, egeAtomicCompareAndSet(m_start, 1, 0));

  createThreads(KThreadCount, AtomicOperationsTest::CompareAndSet);
  waitUntilThreadsFinish();

  EXPECT_EQ(KThreadCount * KRepetitionsCount, m_value);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------