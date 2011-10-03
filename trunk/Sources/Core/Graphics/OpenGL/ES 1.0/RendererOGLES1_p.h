#ifndef EGE_CORE_RENDERER_PRIVATE_H
#define EGE_CORE_RENDERER_PRIVATE_H

#include <EGE.h>
#include <EGEMap.h>
#include <EGEDynamicArray.h>
#include "GLES/gl.h"
#include "Core/Graphics/Render/Renderer.h"
#include "Core/Components/Render/RenderComponent.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Renderer;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class RendererPrivate
{
  public:

    RendererPrivate(Renderer* base);
   ~RendererPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(Renderer)

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Sets given viewport. */
    void setViewport(const PViewport& viewport);
    /* Clears given viewport. */
    void clearViewport(const PViewport& viewport);
    /* Sends all geometry through the geometry pipeline to hardware. */
    void flush();

  private:

    /* Detects rendering capabilities. */
    void detectCapabilities();
    /* Activates given texture unit. */
    bool activateTextureUnit(u32 unit);
    /* Checks if given extension is supported. */
    bool isExtensionSupported(const char* extension) const;
    /* Binds texture to target. */
    bool bindTexture(GLenum target, GLuint textureId);
    /* Sets render target. */
    void setRenderTarget(const PRenderTarget& renderTarget);
    /* Applies general parameters. 
     *  @note General parameters are the ones that require only one setup before component is rendered. 
     */
    void applyGeneralParams(const PRenderComponent& component);
    /* Applies parameters for given pass. */
    void applyPassParams(const PRenderComponent& component, const PMaterial& material, const RenderPass* pass);

  private:

    /*! Texture unit currently active. */
    u32 m_activeTextureUnit;
    /*! Pool of all currently active texture units. */
    DynamicArray<u32> m_activeTextureUnits;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RENDERER_PRIVATE_H