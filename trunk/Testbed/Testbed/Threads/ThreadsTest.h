#ifndef TEST_THREADS_H
#define TEST_THREADS_H

#include "Test.h"

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
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // TEST_THREADS_H