#ifndef RIPPLEEFFECT_H
#define RIPPLEEFFECT_H

#include <EGETexture.h>
#include <EGEScene.h>
#include <EGEGraphics.h>
#include <EGETime.h>
#include <EGEInput.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class RippleEffect : public EGE::SceneNodeObject
{
  public:

    RippleEffect(EGE::Application* app);
    virtual ~RippleEffect();

    /* Initializes object. */
    bool initialize(EGE::s32 width, EGE::s32 height, EGE::PCamera camera);
    /*! Returns render texture. */
    EGE::PTexture2D texture() const { return m_texture; }
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
    /*! Render texture. */
    EGE::PTexture2D m_texture;



    EGE::s32 m_maxRipple;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RIPPLEEFFECT_H