#ifndef EGE_CORE_GRAPHICS_RENDER_COMPONENTRENDERER_H
#define EGE_CORE_GRAPHICS_RENDER_COMPONENTRENDERER_H

/*! @brief This class represents interface for component rendering for use of any Render System. 
 */

#include "EGE.h"
#include "EGEMatrix.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(RenderComponent, PRenderComponent)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IComponentRenderer
{
  public:

    IComponentRenderer() {}
    virtual ~IComponentRenderer() {}

    /*! Renders given component. 
     *  @param  component   Component to render.
     *  @param  modelMatrix Model transformation matrix.
     */
    virtual void renderComponent(const PRenderComponent& component, const Matrix4f& modelMatrix) = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_RENDER_COMPONENTRENDERER_H