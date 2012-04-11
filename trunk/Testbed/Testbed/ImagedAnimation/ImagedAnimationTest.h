#ifndef TEST_IMAGED_ANIMATION_H
#define TEST_IMAGED_ANIMATION_H

#include "Test.h"
#include <EGEEvent.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ImagedAnimationTest : public Test, public EGE::IEventListener
{
  public:

    ImagedAnimationTest(App* app);
    virtual ~ImagedAnimationTest();

    /* Test override. Returns test name. */
    EGE::String name() const override;
    /* Test override. Initializes test. */
    bool initialize() override;
    /* Test override. Updates test. */
    void update(const EGE::Time& time) override;

  private:

    /* Test override. Pointer event receiver. */
    void pointerEvent(EGE::PPointerData data) override;
    /* IEventListener override. Event reciever. */
    void onEventRecieved(EGE::PEvent event) override;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // TEST_IMAGED_ANIMATION_H