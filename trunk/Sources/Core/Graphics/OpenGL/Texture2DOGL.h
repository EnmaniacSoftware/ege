#ifndef EGE_CORE_TEXTURE2D_PRIVATE_H
#define EGE_CORE_TEXTURE2D_PRIVATE_H

#include <EGE.h>
#include <EGETexture.h>

#ifdef EGE_PLATFORM_WIN32
#include <gl/GL.h>
#else
#include "GLES/gl.h"
#endif

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Texture2DPrivate
{
  public:

    Texture2DPrivate(Texture2D* base);
   ~Texture2DPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Creates texture from given file. */
    EGEResult create(const String& path);
    /*! Returns texture ID. */
    inline GLuint id() const { return m_id; }

  private:

    /* Maps filter to OpenGL equivalent. */
    GLint mapFilter(EGETexture::Filter filter) const;
    /* Maps texture addressing mode into OpenGL equivalent. */
    GLint mapAddressingMode(EGETexture::AddressingMode mode) const;

  private:

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(Texture2D);

    /*! Texture ID. */
    GLuint m_id;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_TEXTURE2D_PRIVATE_H
