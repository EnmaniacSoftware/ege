#ifndef EGE_CORE_TEXTURE2D_PRIVATE_H
#define EGE_CORE_TEXTURE2D_PRIVATE_H

#include "EGE.h"
#include "EGETexture.h"
#include "EGEOpenGL.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Texture2DPrivate
{
  /* For accessing private data. */
  friend class RenderSystemOGL;

  public:

    Texture2DPrivate(Texture2D* base);
   ~Texture2DPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! @see Texture2D::isValid. */
    bool isValid() const;
    /*! @see Texture2D::create. */
    EGEResult create(const String& path);
    /*! @see Texture2D::create. */
    EGEResult create(const PDataBuffer& buffer);
    /*! @see Texture2D::create. */
    EGEResult create(const PImage& image);
    /*! Returns texture ID. */
    GLuint id() const { return m_id; }

  private:

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(Texture2D);

    /*! Texture ID. */
    GLuint m_id;
    /*! Texture internal format. */
    GLenum m_internalFormat;
    /*! Texture type format. */
    GLenum m_typeFormat;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_TEXTURE2D_PRIVATE_H
