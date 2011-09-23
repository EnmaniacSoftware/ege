#ifndef TEST_LIGHTNING_H
#define TEST_LIGHTNING_H

#include "Test.h"
#include "RenderObject.h"
#include "LightningEffect.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class LightningTest : public Test
{
  public:

    LightningTest(App* app);
    virtual ~LightningTest();

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
    /*! Lightning effect object. */
    LightningEffect* m_effect;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // TEST_LIGHTNING_H