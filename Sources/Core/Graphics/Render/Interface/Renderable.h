#ifndef EGE_CORE_GRAPHICS_RENDER_RENDERABLE_H
#define EGE_CORE_GRAPHICS_RENDER_RENDERABLE_H

/*! Interface for renderable objects. 
 *  Such object renders themselves through given renderer.
 */

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IRenderer;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Renderable
{
  public:

    Renderable() {}
    virtual ~Renderable() {}

    /*! Adds render data for rendering. 
     *  @param  renderer  Renderer to be used for rendering of this object.
     *  @return EGE_SUCCESS on success. EGE_ERROR_NOT_SUPPORTED if component has not been accepted by renderer. Otherwise, EGE_ERROR.
     */
    virtual EGEResult addForRendering(IRenderer& renderer) = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_RENDER_RENDERABLE_H