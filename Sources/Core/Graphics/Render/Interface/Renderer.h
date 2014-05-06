#ifndef EGE_CORE_GRAPHICS_RENDER_RENDERER_H
#define EGE_CORE_GRAPHICS_RENDER_RENDERER_H

/*! @brief This class represents public interface of any renderer. */

#include "EGE.h"
#include "EGEMatrix.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(RenderComponent, PRenderComponent)
EGE_DECLARE_SMART_CLASS(RenderQueue, PRenderQueue)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IRenderer
{
  public:

    IRenderer() {}
    virtual ~IRenderer() {}

    /*! Adds given data for rendering. 
     *  @param  component   Render component which is to be added for rendering.
     *  @param  worldMatrix Model view to world transformation matrix.
     *  @return EGE_SUCCESS on success. EGE_ERROR_NOT_SUPPORTED if component has not been accepted by renderer. Otherwise, EGE_ERROR.
     */
    virtual EGEResult addForRendering(const PRenderComponent& component, const Matrix4f& worldMatrix = Matrix4f::IDENTITY) = 0;
    /*! Adds given render queue for rendering. 
     *  @param  queue Render queue which is to be added for rendering.
     *  @return EGE_SUCCESS on success. EGE_ERROR_NOT_SUPPORTED if queue has not been accepted by renderer. Otherwise, EGE_ERROR.
     */
    virtual EGEResult addForRendering(const PRenderQueue& queue) = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_RENDER_RENDERER_H