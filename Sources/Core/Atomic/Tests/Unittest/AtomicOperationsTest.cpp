#include "TestFramework/Interface/TestBase.h"
#include <EGEAtomic.h>
#include <EGEDevice.h>
#include <pthread.h>
#include <algorithm>
#include <vector>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const s32 KRepetitionsCount = 10000;
static const s32 KThreadCount      = 20;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef void* (*threadMain)(void* userData);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
struct ThreadArgs
{
  void* data;
  int index;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AtomicOperationsTest : public TestBase
{
  protected:

    /*! Thread main function for increments. */
    static void* Increment(void* userData);
    /*! Thread main function for decrements. */
    static void* Decrement(void* userData);
    /*! Thread main function for arithmentic addition. */
    static void* Add(void* userData);
    /*! Thread main function for compares and sets. */
    static void* CompareAndSet(void* userData);
    /*! Thread main function for compares and sets. */
    static void* CompareAndSetPointer(void* userData);
    /*! Thread main function for compares and sets. */
    static void* PointerSet(void* userData);

  protected:

    virtual void SetUp();
    virtual void TearDown();

    void createThreads(s32 count, threadMain main);
    void startThreads();
    void waitUntilThreadsFinish();

  protected:

    /*! Pool of threads. */
    std::vector<pthread_t> m_thread;
    /*! Large array of values to be modifed. */
    std::vector<void*> m_largeValues;
    /*! Pointer value to be modified. */
    void* m_valuePointer;
    /*! Value to be modifed. */
    volatile s32 m_value;
    /*! Global thread activation flag. */
    volatile s32 m_start;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* AtomicOperationsTest::Increment(void* userData)
{
  ThreadArgs* args = reinterpret_cast<ThreadArgs*>(userData);

  AtomicOperationsTest* me = reinterpret_cast<AtomicOperationsTest*>(args->data);

  while (0 == me->m_start)
  {
    Device::Sleep(1);
  }

  for (s32 i = 0; i < KRepetitionsCount; ++i)
  {
    egeAtomicIncrement(me->m_value);
  }

  delete args;

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* AtomicOperationsTest::Decrement(void* userData)
{
  ThreadArgs* args = reinterpret_cast<ThreadArgs*>(userData);

  AtomicOperationsTest* me = reinterpret_cast<AtomicOperationsTest*>(args->data);

  while (0 == me->m_start)
  {
    Device::Sleep(1);
  }

  for (s32 i = 0; i < KRepetitionsCount; ++i)
  {
    egeAtomicDecrement(me->m_value);
  }

  delete args;

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* AtomicOperationsTest::Add(void* userData)
{
  ThreadArgs* args = reinterpret_cast<ThreadArgs*>(userData);

  AtomicOperationsTest* me = reinterpret_cast<AtomicOperationsTest*>(args->data);

  while (0 == me->m_start)
  {
    Device::Sleep(1);
  }

  for (s32 i = 0; i < KRepetitionsCount; ++i)
  {
    egeAtomicAdd(me->m_value, 1);
  }

  delete args;

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* AtomicOperationsTest::CompareAndSet(void* userData)
{
  ThreadArgs* args = reinterpret_cast<ThreadArgs*>(userData);

  AtomicOperationsTest* me = reinterpret_cast<AtomicOperationsTest*>(args->data);

  for (s32 i = 0; i < KRepetitionsCount; ++i)
  {
    while (0 != egeAtomicCompareAndSet(me->m_start, 0, 1))
    {
      // wait
      Device::Sleep(1);
    }

    // NOTE: all code from here till another egeAtomicCompareAndSet is a kind of critical section, no other thread should be able to share this with some other
    ++me->m_value;

    EXPECT_EQ(1, egeAtomicCompareAndSet(me->m_start, 1, 0));
  }

  delete args;

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* AtomicOperationsTest::CompareAndSetPointer(void* userData)
{
  ThreadArgs* args = reinterpret_cast<ThreadArgs*>(userData);

  AtomicOperationsTest* me = reinterpret_cast<AtomicOperationsTest*>(args->data);

  for (s32 i = 0; i < KRepetitionsCount; ++i)
  {
    while (NULL != egeAtomicCompareAndSet(me->m_valuePointer, NULL, reinterpret_cast<void*>(1)))
    {
      // wait
      Device::Sleep(1);
    }

    // NOTE: all code from here till another egeAtomicCompareAndSet is a kind of critical section, no other thread should be able to share this with some other
    ++me->m_value;

    EXPECT_EQ(reinterpret_cast<void*>(1), egeAtomicCompareAndSet(me->m_valuePointer, reinterpret_cast<void*>(1), NULL));
  }

  delete args;

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* AtomicOperationsTest::PointerSet(void* userData)
{
  ThreadArgs* args = reinterpret_cast<ThreadArgs*>(userData);

  AtomicOperationsTest* me = reinterpret_cast<AtomicOperationsTest*>(args->data);

  const s32 base = args->index * KRepetitionsCount;

  // atomically set value by value in this thread's area of input array
  for (s32 i = (base + 1); i <= (base + KRepetitionsCount); ++i)
  {
    void* oldValue = egeAtomicSet(me->m_valuePointer, reinterpret_cast<void*>(i)); 
    
    me->m_largeValues[i] = oldValue;
  }

  delete args;

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AtomicOperationsTest::SetUp()
{
  m_value = 0;
  m_start = 0;
  m_valuePointer = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AtomicOperationsTest::TearDown()
{
  m_thread.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AtomicOperationsTest::createThreads(s32 count, threadMain main)
{
  for (s32 i = 0; i < count; ++i)
  {
    pthread_t thread;
    
    ThreadArgs* args = new ThreadArgs();
    args->data = this;
    args->index = i;

    EXPECT_EQ(0, pthread_create(&thread, NULL, main, args));
  
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
TEST_F(AtomicOperationsTest, Increment_S32)
{
  createThreads(KThreadCount, AtomicOperationsTest::Increment);
  startThreads();
  waitUntilThreadsFinish();

  EXPECT_EQ(KThreadCount * KRepetitionsCount, m_value);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AtomicOperationsTest, Decrement_S32)
{
  createThreads(KThreadCount, AtomicOperationsTest::Decrement);
  startThreads();
  waitUntilThreadsFinish();

  EXPECT_EQ(-KThreadCount * KRepetitionsCount, m_value);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AtomicOperationsTest, Add_S32)
{
  createThreads(KThreadCount, AtomicOperationsTest::Add);
  startThreads();
  waitUntilThreadsFinish();

  EXPECT_EQ(KThreadCount * KRepetitionsCount, m_value);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AtomicOperationsTest, CompareAndSet_S32)
{
  createThreads(KThreadCount, AtomicOperationsTest::CompareAndSet);
  waitUntilThreadsFinish();

  EXPECT_EQ(KThreadCount * KRepetitionsCount, m_value);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AtomicOperationsTest, Set_Pointer)
{
  // NOTE: make some room the the begining (at index 0) for the final value once all threads are finished
  m_largeValues.resize(KThreadCount * KRepetitionsCount + 1, NULL);
  EXPECT_EQ(KThreadCount * KRepetitionsCount + 1, m_largeValues.size());

  createThreads(KThreadCount, AtomicOperationsTest::PointerSet);
  waitUntilThreadsFinish();

  // store the current (last) value within an array
  m_largeValues[0] = m_valuePointer;

  // store in ascending order
  std::sort(m_largeValues.begin(), m_largeValues.end());

  // check data integrity (there should be no gaps or repetitions)
  for (s32 i = 0; i < KThreadCount * KRepetitionsCount; ++i)
  {
    EXPECT_EQ(reinterpret_cast<void*>(i), m_largeValues[i]) << " where i = " << i;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(AtomicOperationsTest, CompareAndSet_Pointer)
{
  createThreads(KThreadCount, AtomicOperationsTest::CompareAndSetPointer);
  waitUntilThreadsFinish();

  EXPECT_EQ(KThreadCount * KRepetitionsCount, m_value);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------