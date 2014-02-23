#ifndef TEST_SOUND_H
#define TEST_SOUND_H

#include "Test.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class SoundTest : public Test
{
  public:

    SoundTest(App* app);
    virtual ~SoundTest();

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

#endif // TEST_SOUND_H