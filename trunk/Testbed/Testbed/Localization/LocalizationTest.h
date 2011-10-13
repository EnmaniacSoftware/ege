#ifndef TEST_LOCALIZATION_H
#define TEST_LOCALIZATION_H

#include "Test.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class LocalizationTest : public Test
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

  private:

};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // TEST_LOCALIZATION_H