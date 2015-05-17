#include "TestFramework/Interface/TestBase.h"
#include <EGEAtomicBool.h>
#include <EGEAtomicInt.h>
#include <EGEDevice.h>
#include <pthread.h>
#include <algorithm>
#include <vector>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const s32 KRepetitionsCount = 100000;
static const s32 KThreadCount      = 20;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef void* (*threadMain)(void* userData);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AtomicBoolTest;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
struct ThreadArgs
{
  AtomicBoolTest* test;
  int index;
  bool loadThread;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AtomicBoolTest : public TestBase
{
  protected:

    /*! Thread main function for load/store testing. */
    static void* LoadAndStore(void* userData);

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
    /*! Counter. */
    AtomicInt m_counter;
    /*! Global thread activation flag. */
    bool m_start;
    /*! Global thread termination flag. */
    bool m_done;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* AtomicBoolTest::LoadAndStore(void* userData)
{
  ThreadArgs* args = reinterpret_cast<ThreadArgs*>(userData);

  while ( ! args->test->m_start)
  {
    Device::Sleep(1);
  }

  if (args->loadThread)
  {
    for (s32 i = 0; i < KRepetitionsCount; ++i)
    {
      // check if flag is set
      while ( ! args->test->m_value.load())
      {
      }

      // TRUE

      // reset the flag
      EXPECT_TRUE(args->test->m_value.store(false));
    }

    // signal end of work
    args->test->m_done = true;
  }
  else
  {
    while (true)
    {
      // try to set the flag
      // NOTE: only one non-loading thread can do this at a time. Otherwise, we can expect the counter to be incremented too many times.
      while (args->test->m_value.store(true))
      {
        if (args->test->m_done)
        {
          delete args;
          pthread_exit(0);
        }
      }

      // TRUE

      // flag has been set, increase the counter
      EXPECT_GE(KRepetitionsCount, args->test->m_counter.increment());
    }
  }

  delete args;

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AtomicBoolTest::SetUp()
{
  m_value = AtomicBool(true);
  m_start = false;
  m_done  = false;
  m_counter = AtomicInt(0);
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

    ThreadArgs* args = new ThreadArgs();
    args->test = this;
    args->index = i;
    args->loadThread = (i == 0);  // only 1 'load' thread

    EXPECT_EQ(0, pthread_create(&thread, NULL, main, args));
  
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
TEST_F(AtomicBoolTest, LoadAndStore)
{
  createThreads(KThreadCount, LoadAndStore);
  startThreads();
  waitUntilThreadsFinish();

  EXPECT_EQ(KRepetitionsCount, m_counter.load());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
