#ifndef EGE_CORE_GRAPHICS_OPENGL_TEXTURE2DOGL_H
#define EGE_CORE_GRAPHICS_OPENGL_TEXTURE2DOGL_H

#include "EGE.h"
#include "EGEOpenGL.h"
#include "Core/Graphics/Texture2D.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Texture2DOGL : public Texture2D
{
  public:

    Texture2DOGL(Engine& engine, const String& name, IHardwareResourceProvider* provider);
   ~Texture2DOGL();

  public:

    /*! @see Texture2D::create. */
    EGEResult create(const String& path) override;
    /*! @see Texture2D::create. */
    EGEResult create(const PDataBuffer& buffer) override;
    /*! @see Texture2D::create. */
    EGEResult create(const PImage& image) override;
    /*! Returns texture ID. */
    GLuint id() const;

  private:

    /*! Texture ID. */
    GLuint m_id;
    /*! Texture internal format. */
    GLenum m_internalFormat;
    /*! Texture type format. */
    GLenum m_typeFormat;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_OPENGL_TEXTURE2DOGL_H
