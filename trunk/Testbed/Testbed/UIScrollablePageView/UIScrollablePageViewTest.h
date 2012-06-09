#ifndef TEST_UISCROLLABLEPAGEVIEW_H
#define TEST_UISCROLLABLEPAGEVIEW_H

#include "Test.h"
#include <EGEString.h>
#include <EGEUI.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RenderObject;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class UIScrollablePageViewTest : public Test
{
  public:

    UIScrollablePageViewTest(App* app);
    virtual ~UIScrollablePageViewTest();

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

    /*! Scrollable page view object. */
    EGE::PUIScrollablePageView m_area;
    /*! Scrollable area background object. */
    EGE::PRenderComponent m_areaBackground;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // TEST_UISCROLLABLEPAGEVIEW_H