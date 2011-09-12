#ifndef TEST_RENDER_TO_TEXTURE_H
#define TEST_RENDER_TO_TEXTURE_H

#include "Test.h"
#include "RenderObject.h"
#include <EGETexture.h>
#include <EGEGraphics.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class RenderToTextureTest : public Test
{
  public:

    RenderToTextureTest(App* app);
    virtual ~RenderToTextureTest();

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
    /*! Scene object mapped with render texture. */
    RenderObject* m_sceneObjectRenderTexture;
    /*! Render texture. */
    EGE::PTexture2D m_texture;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // TEST_RENDER_TO_TEXTURE_H