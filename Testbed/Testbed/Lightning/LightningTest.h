#ifndef TEST_LIGHTNING_H
#define TEST_LIGHTNING_H

#include "Test.h"
#include "RenderObject.h"
#include "LightningEffect.h"
#include "LightningEffectLines.h"
#include "LightningEffectQuads.h"
#include "LightningEffectStrips.h"

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
    /* Test override. Slot called when resource group has been loaded. */
    void groupLoadComplete(const EGE::String& name) override;

  private:

    /*! Scene object. */
    RenderObject* m_sceneObject;
    /*! Lightning effect object. */
    LightningEffect* m_effect;
    /*! Lightning effect (based on lines) object. */
    LightningEffectLines* m_effectLines;
    /*! Lightning effect (based on quads) object. */
    LightningEffectQuads* m_effectQuads;
    /*! Lightning effect (based on strips) object. */
    LightningEffectStrips* m_effectStrips;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // TEST_LIGHTNING_H