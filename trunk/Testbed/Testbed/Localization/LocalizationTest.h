#ifndef TEST_LOCALIZATION_H
#define TEST_LOCALIZATION_H

#include "Test.h"
#include <EGEEvent.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class LocalizationTest : public Test, public EGE::IEventListener
{
  public:

    LocalizationTest(App* app);
    virtual ~LocalizationTest();

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
    /* Updates texts. */
    void updateTexts();
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // TEST_LOCALIZATION_H