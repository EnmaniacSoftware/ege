#include "TestFramework/Interface/TestBase.h"
#include <EGEAtomicBool.h>
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
class AtomicBoolTest : public TestBase
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
    AtomicBool m_value;
    /*! Global thread activation flag. */
    bool m_start;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* AtomicBoolTest::Increment(void* userData)
{
  AtomicBoolTest* me = reinterpret_cast<AtomicBoolTest*>(userData);

  while ( ! me->m_start)
  {
    AtomicBoolTest::OsSleep(1);
  }

  for (s32 i = 0; i < KRepetitionsCount; ++i)
  {
    //me->m_value.increment();
  }

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* AtomicBoolTest::Decrement(void* userData)
{
  AtomicBoolTest* me = reinterpret_cast<AtomicBoolTest*>(userData);

  while ( ! me->m_start)
  {
    AtomicBoolTest::OsSleep(1);
  }

  for (s32 i = 0; i < KRepetitionsCount; ++i)
  {
   // me->m_value.decrement();
  }

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AtomicBoolTest::OsSleep(u32 miliseconds)
{
  Sleep(miliseconds);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AtomicBoolTest::SetUp()
{
  m_value = AtomicBool(false);
  m_start = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AtomicBoolTest::TearDown()
{
  m_thread.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AtomicBoolTest::createThreads(s32 count, threadMain main)
{
  for (s32 i = 0; i < count; ++i)
  {
    pthread_t thread;
    EXPECT_EQ(0, pthread_create(&thread, NULL, main, this));
  
    m_thread.push_back(thread);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AtomicBoolTest::startThreads()
{
  m_start = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AtomicBoolTest::waitUntilThreadsFinish()
{
  for (std::vector<pthread_t>::const_iterator it = m_thread.begin(); it != m_thread.end(); ++it)
  {
    const pthread_t& thread = *it;

    void* status;
    EXPECT_EQ(0, pthread_join(thread, &status));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AtomicBoolTest, Load)
{
  // TAGE - not sure how to test it
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AtomicBoolTest, Store)
{
  // TAGE - not sure how to test it
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------