#ifndef EGE_CORE_GRAPHICS_RENDER_RENDERSYSTEM_INTERFACE_H
#define EGE_CORE_GRAPHICS_RENDER_RENDERSYSTEM_INTERFACE_H

#include "EGE.h"
#include "EGEMatrix.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_PREDECLARE_SMART_CLASS(Viewport, PViewport)
EGE_PREDECLARE_SMART_CLASS(RenderPass, PRenderPass)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IRenderSystem
{
  public:

    virtual ~IRenderSystem() {}

  public:

    /*! Sends all geometry through the geometry pipeline to hardware. */
    virtual void flush() = 0;
    /*! Clears given viewport. 
     *  @param  viewport  Viewport to clear.
     */
    virtual void clearViewport(const PViewport& viewport) = 0;
    /*! Sets given viewport. 
     *  @param  viewport  Viewport to set.
     */
    virtual void setViewport(const PViewport& viewport) = 0;

    /*! Sets view matrix. 
     *  @param  matrix  View matrix.
     */
    virtual void setViewMatrix(const Matrix4f& matrix) = 0;
    /*! Returns view matrix. */
    virtual const Matrix4f& viewMatrix() const = 0;
    /*! Sets projection matrix. 
     *  @param  matrix  Project matrix.
     */
    virtual void setProjectionMatrix(const Matrix4f& matrix) = 0;

    /*! Sets texture minifying function filter. 
     *  @param  filter  Texture minifying filter to set.
     */
    virtual void setTextureMinFilter(TextureFilter filter) = 0;
    /*! Sets texture magnification function filter. 
     *  @param  filter  Texture magnification filter to set.
     */
    virtual void setTextureMagFilter(TextureFilter filter) = 0;
    /*! Sets texture addressing mode for S texture coordinate. 
     *  @param  mode  Texture addressing mode for S coordinate.
     */
    virtual void setTextureAddressingModeS(TextureAddressingMode mode) = 0;
    /*! Sets texture addressing mode for T texture coordinate. 
     *  @param  mode  Texture addressing mode for T coordinate.
     */
    virtual void setTextureAddressingModeT(TextureAddressingMode mode) = 0;
    /*! Enables/disables texture mipmapping for newly created textures. 
     *  @param  set If set, enables mipmapping.
     */
    virtual void setTextureMipMapping(bool set) = 0;

  protected:

    /*! Applies pass specific parameters.
     *  @param  pass      Render pass currently being processed.
     */
    virtual void applyPassParams(const PRenderPass& pass) = 0;
    /*! Applies generic parameters for component currently being rendered. */
    virtual void applyGeneralParams() = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_RENDER_RENDERSYSTEM_INTERFACE_H