#ifndef EGE_CORE_GRAPHICS_RENDER_RENDERER_H
#define EGE_CORE_GRAPHICS_RENDER_RENDERER_H

/*! @brief This class represents public interface of any renderer. */

#include "EGE.h"
#include "EGEMatrix.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(RenderComponent, PRenderComponent)
EGE_DECLARE_SMART_CLASS(RenderQueue, PRenderQueue)
EGE_DECLARE_SMART_CLASS(RenderTarget, PRenderTarget)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IRenderer
{
  public:

    IRenderer() {}
    virtual ~IRenderer() {}

    /*! Adds given data for rendering. */
    virtual bool addForRendering(const PRenderComponent& component, const Matrix4f& worldMatrix = Matrix4f::IDENTITY) = 0;
    /*! Adds given render queue for rendering. */
    virtual bool addForRendering(const PRenderQueue& queue) = 0;
    /*! Returns view matrix. */
    virtual const Matrix4f& viewMatrix() const = 0;
    /*! Returns number of batches rendered last frame. */
    virtual u32 batchCount() const = 0;
    /*! Returns number of vertices rendered last frame. */
    virtual u32 vertexCount() const = 0;
    /*! Returns current render target. */
    virtual PRenderTarget currentRenderTarget() const = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_RENDER_RENDERER_H