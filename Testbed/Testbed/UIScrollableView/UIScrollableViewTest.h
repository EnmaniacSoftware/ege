#ifndef TEST_SCROLLABLEAREA_H
#define TEST_SCROLLABLEAREA_H

#include "Test.h"
#include <EGEString.h>
#include <EGEUI.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class RenderObject;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class UIScrollableViewTest : public Test
{
  public:

    UIScrollableViewTest(App* app);
    virtual ~UIScrollableViewTest();

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

    /*! Scrollable area object. */
    EGE::PUIScrollableView m_area;
    /*! Scrollable area background object. */
    EGE::PRenderComponent m_areaBackground;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // TEST_SCROLLABLEAREA_H