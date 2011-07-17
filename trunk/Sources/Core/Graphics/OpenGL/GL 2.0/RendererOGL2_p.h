#ifndef EGE_CORE_RENDERER_PRIVATE_H
#define EGE_CORE_RENDERER_PRIVATE_H

#include <EGE.h>
#include <EGEMap.h>
#include <EGEDynamicArray.h>
#include <gl/gl.h>
#include "Core/Graphics/Render/Renderer.h"
#include "Core/Components/Render/RenderComponent.h"
#include "Core/Graphics/OpenGL/GL 2.0/glext.h"

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
    /* Clears given viewport. */
    void clearViewport(const PViewport& viewport);
    /* Sends all geometry through the geometry pipeline to hardware. */
    void flush();
    /* Applies material. */
    void applyMaterial(const PMaterial& material);

  private:

    /* Detects rendering capabilities. */
    void detectCapabilities();
    /* Activates given texture unit. */
    bool activateTextureUnit(u32 unit);
    /* Checks if given extension is supported. */
    bool isExtensionSupported(const char* extension) const;
    /* Binds texture to target. */
    bool bindTexture(GLenum target, GLuint textureId);

  private:

    /*! Texture unit currently active. */
    u32 m_activeTextureUnit;
    /*! Map of currently bound textures to targets. */
    Map<GLenum, GLuint> m_boundTextures;
    /*! Pool of all currently active texture units. */
    DynamicArray<u32> m_activeTextureUnits;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RENDERER_PRIVATE_H