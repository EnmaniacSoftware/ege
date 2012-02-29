#ifndef TEST_THREADS_H
#define TEST_THREADS_H

#include "Test.h"
#include "TestThread.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define THREADS_COUNT 1
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ThreadsTest : public Test
{
  public:

    ThreadsTest(App* app);
    virtual ~ThreadsTest();

    /* Test override. Returns test name. */
    EGE::String name() const override;
    /* Test override. Initializes test. */
    bool initialize() override;
    /* Test override. Updates test. */
    void update(const EGE::Time& time) override;

  private:

    /* Test override. Pointer event receiver. */
    void pointerEvent(EGE::PPointerData data) override;

  private slots:

    /* Slot called then thread has finished. */
    void onThreadFinished(const EGE::Thread* thread);
    /* Slot called then thread has started. */
    void onThreadStarted(const EGE::Thread* thread);

  private:

    EGE::PThread m_threads[THREADS_COUNT];
    EGE::s32 m_counters[THREADS_COUNT];
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // TEST_THREADS_H