#include "TestFramework/Interface/TestBase.h"
#include <EGEAtomicPointer.h>
#include <EGEDevice.h>
#include <pthread.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const s32 KRepetitionsCount = 1000000;
static const s32 KThreadCount      = 20;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef void* (*threadMain)(void* userData);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AtomicPointerTest;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
struct ThreadArgs
{
  AtomicPointerTest* test;
  int index;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AtomicPointerTest : public TestBase
{
  protected:

    /*! Thread main function for setting/getting the pointer value (32-bit). */
    static void* LoadAndStore32bit(void* userData);
    /*! Thread main function for setting/getting the pointer value (64-bit). */
    static void* LoadAndStore64bit(void* userData);

  protected:

    virtual void SetUp();
    virtual void TearDown();

    void createThreads(s32 count);
    void startThreads();
    void waitUntilThreadsFinish();

  protected:

    /*! Pool of threads. */
    std::vector<pthread_t> m_thread;
    /*! Value to be modifed. */
    AtomicPointer<void> m_value;
    /*! Global thread activation flag. */
    bool m_start;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* AtomicPointerTest::LoadAndStore32bit(void* userData)
{
  ThreadArgs* args = reinterpret_cast<ThreadArgs*>(userData);

  const u32 KValueLimit  = 0xffffff00u;

  // These values all satisfy x * x >= KValueLimit:
  const u32 KValues[] =
  {
    0x37b91364u,
    0x1c5970efu,
    0x536c76bau,
    0x0a10207fu,
    0x71043c77u,
    0x4db84a83u,
    0x27cf0273u,
    0x74a15a69u
  };

  // initial value needs to be valid
  args->test->m_value.store(reinterpret_cast<void*>(KValues[0]));

  while ( ! args->test->m_start)
  {
    Device::Sleep(1);
  }

  if (0 == (args->index & 0x1))
  {
    u32 index = 0;
    for (s32 i = 0; i < KRepetitionsCount; ++i)
    {
      // calculate next index
      index = (index * index + 1) % 65521;
      
      // store value 
      args->test->m_value.store(reinterpret_cast<void*>(KValues[index & 7]));
    }
  }
  else
  {
    for (s32 i = 0; i < KRepetitionsCount; ++i)
    {
      // load value
      u32 value = reinterpret_cast<u32>(args->test->m_value.load());

      // check if in range
      EXPECT_LE(KValueLimit, value * value) << "for value: " << value;
    }
  }

  delete args;

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* AtomicPointerTest::LoadAndStore64bit(void* userData)
{
  ThreadArgs* args = reinterpret_cast<ThreadArgs*>(userData);

  static const u64 KValueLimit = 0xffffff0000000000ull;

  // These values all satisfy x * x >= KValueLimit:
  const u64 KValues[] =
  {
    0x3c116d2362a21633ull,
    0x7747508552ab6bc6ull,
    0x289a0e1528a43422ull,
    0x15e36d0a61d326eaull,
    0x3ccb2e8c0c6224c4ull,
    0x074504c13a1716e1ull,
    0x6c82417a3ad77b24ull,
    0x3124440040454919ull
  };

  // initial value needs to be valid
  args->test->m_value.store(reinterpret_cast<void*>(KValues[0]));

  while ( ! args->test->m_start)
  {
    Device::Sleep(1);
  }

  if (0 == (args->index & 0x1))
  {
    u32 index = 0;
    for (s32 i = 0; i < KRepetitionsCount; ++i)
    {
      // calculate next index
      index = (index * index + 1) % 65521;
      
      // store value 
      args->test->m_value.store(reinterpret_cast<void*>(KValues[index & 7]));
    }
  }
  else
  {
    for (s32 i = 0; i < KRepetitionsCount; ++i)
    {
      // load value
      u64 value = reinterpret_cast<u64>(args->test->m_value.load());

      // check if in range
      EXPECT_LE(KValueLimit, value * value) << "for value: " << value;
    }
  }

  delete args;

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AtomicPointerTest::SetUp()
{
  m_value = AtomicPointer<void>(NULL);
  m_start = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AtomicPointerTest::TearDown()
{
  m_thread.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AtomicPointerTest::createThreads(s32 count)
{
  for (s32 i = 0; i < count; ++i)
  {
    pthread_t thread;

    ThreadArgs* args = new ThreadArgs();
    args->test = this;
    args->index = i;

    EXPECT_EQ(0, pthread_create(&thread, NULL, (4 == sizeof (void*)) ? AtomicPointerTest::LoadAndStore32bit : AtomicPointerTest::LoadAndStore64bit, args));
  
    m_thread.push_back(thread);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AtomicPointerTest::startThreads()
{
  m_start = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AtomicPointerTest::waitUntilThreadsFinish()
{
  for (std::vector<pthread_t>::const_iterator it = m_thread.begin(); it != m_thread.end(); ++it)
  {
    const pthread_t& thread = *it;

    void* status;
    EXPECT_EQ(0, pthread_join(thread, &status));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AtomicPointerTest, LoadAndStore)
{
  EXPECT_TRUE((4 == sizeof (void*)) || (8 == sizeof (void*)));

  createThreads(KThreadCount);
  startThreads();
  waitUntilThreadsFinish();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------