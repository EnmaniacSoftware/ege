#ifndef EGE_CORE_TEXTURE2D_H
#define EGE_CORE_TEXTURE2D_H

#include "EGE.h"
#include "EGETexture.h"

#ifdef EGE_PLATFORM_WIN32
#include <gl/GL.h>
#else
#include "GLES/gl.h"
#endif

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(Texture2D, PTexture2D)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Texture2D : public Object
{
  public:

    Texture2D(Application* app, EGETexture::Filter minFilter = EGETexture::BILINEAR, EGETexture::Filter magFilter = EGETexture::BILINEAR,
              EGETexture::Wrap wrapS = EGETexture::REPEAT, EGETexture::Wrap wrapT = EGETexture::REPEAT);
   ~Texture2D();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Creates texture from given file. */
    EGEResult create(const EGEString& path);
    /* Sets minifying function filter. */
    void setMinFilter(EGETexture::Filter filter);
    /* Sets magnification function filter. */
    void setMagFilter(EGETexture::Filter filter);
    /* Sets wrap parameter for S coordinate. */
    void setWrapS(EGETexture::Wrap mode);
    /* Sets wrap parameter for T coordinate. */
    void setWrapT(EGETexture::Wrap mode);
    /*! Returns texture ID. */
    inline GLuint id() const { return m_id; }

  private:

    /* Maps filter to OpenGL equivalent. */
    GLint mapFilter(EGETexture::Filter filter) const;
    /* Maps wrap parameter to OpenGL equivalent. */
    GLint mapWrap(EGETexture::Wrap wrap) const;

  private:

    /*! Texture ID. */
    GLuint m_id;
    /*! Minifying function filter. */
    EGETexture::Filter m_minFilter;
    /*! Magnification function filter. */
    EGETexture::Filter m_magFilter;
    /*! Wrap parameter for S coordinate. */
    EGETexture::Wrap m_wrapS;
    /*! Wrap parameter for T coordinate. */
    EGETexture::Wrap m_wrapT;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_TEXTURE2D_H
