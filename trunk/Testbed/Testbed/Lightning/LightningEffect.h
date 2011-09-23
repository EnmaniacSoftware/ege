#ifndef LIGHTNINGEFFECT_H
#define LIGHTNINGEFFECT_H

#include <EGETexture.h>
#include <EGEScene.h>
#include <EGEGraphics.h>
#include <EGETime.h>
#include <EGEInput.h>
#include <EGEDynamicArray.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class LightningEffect : public EGE::SceneNodeObject
{
  public:

    LightningEffect(EGE::Application* app);
    virtual ~LightningEffect();

  public:

    /* Updates effect. */
    void update(const EGE::Time& time);
    /* Pointer event receiver. */
    void pointerEvent(EGE::PPointerData data);

  private:

    /* SceneNodeObject override. Adds object render data for rendering with given renderer. */
    bool addForRendering(EGE::Renderer* renderer) override;

  private:

    /*! Pointer to application object. */
    EGE::Application* m_app;
    /*! Render data. */
    EGE::PRenderComponent m_renderData;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // LIGHTNINGEFFECT_H