#ifndef TEST_RIPPLES_H
#define TEST_RIPPLES_H

#include "Test.h"
#include "RenderObject.h"
#include "RippleEffect.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class RipplesTest : public Test
{
  public:

    RipplesTest(App* app);
    virtual ~RipplesTest();

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

    /* Slot called before target is rendered. */
    void preRender(EGE::PRenderTarget target);
    /* Slot called after target has been rendered. */
    void postRender(EGE::PRenderTarget target);

  private:

    /*! Scene object. */
    RenderObject* m_sceneObject;
    /*! Ripple effect object. */
    RippleEffect* m_rippleEffect;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // TEST_RIPPLES_H