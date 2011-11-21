#ifndef TEST_FONTS_H
#define TEST_FONTS_H

#include "Test.h"
#include <EGEEvent.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class FontsTest : public Test, public EGE::IEventListener
{
  public:

    FontsTest(App* app);
    virtual ~FontsTest();

    /* Test override. Returns test name. */
    EGE::String name() const override;
    /* Test override. Initializes test. */
    bool initialize() override;
    /* Test override. Updates test. */
    void update(const EGE::Time& time) override;

  private:

    /* IEventListener override. Event reciever. */
    void onEventRecieved(EGE::PEvent event) override;
    /* Updates texts. */
    void updateTexts();
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // TEST_FONTS_H