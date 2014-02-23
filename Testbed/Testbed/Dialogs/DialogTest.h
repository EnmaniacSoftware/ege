#ifndef TEST_DIALOG_H
#define TEST_DIALOG_H

#include "Test.h"
#include <EGEUI.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class DialogTest : public Test
{
  public:

    DialogTest(App* app);
    virtual ~DialogTest();

    /* Test override. Returns test name. */
    EGE::String name() const override;
    /* Test override. Initializes test. */
    bool initialize() override;
    /* Test override. Updates test. */
    void update(const EGE::Time& time) override;

  private:

    /* Test override. Pointer event receiver. */
    void pointerEvent(EGE::PPointerData data) override;
    /* Test override. Slot called when resource group has been loaded. */
    void groupLoadComplete(const EGE::String& name) override;

  private slots:

    /* Slot called before target is rendered. */
    void preRender(EGE::PRenderTarget target);

  private:

    /*! Dialog. */
    EGE::PDialog m_dialog;
    EGE::PPushButton m_button;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // TEST_DIALOG_H