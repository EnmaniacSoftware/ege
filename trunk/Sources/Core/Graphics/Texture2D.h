#ifndef EGE_CORE_TEXTURE2D_H
#define EGE_CORE_TEXTURE2D_H

/** Base object representing 2D texture.
*/

#include <EGE.h>
#include <EGETexture.h>
#include <EGEString.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(Texture2D, PTexture2D)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Texture2D : public Object
{
  /* For accessing private data. */
  friend class RendererPrivate;

  public:

    Texture2D(Application* app, EGETexture::Filter minFilter = EGETexture::BILINEAR, EGETexture::Filter magFilter = EGETexture::BILINEAR,
              EGETexture::AddressingMode modeS = EGETexture::AM_REPEAT, EGETexture::AddressingMode modeT = EGETexture::AM_REPEAT);
   ~Texture2D();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Creates texture from given file. */
    EGEResult create(const String& path);
    /* Sets minifying function filter. */
    void setMinFilter(EGETexture::Filter filter);
    /* Sets magnification function filter. */
    void setMagFilter(EGETexture::Filter filter);
    /* Sets texture addressing mode for S texture coordinate. */
    void setTextureAddressingModeS(EGETexture::AddressingMode mode);
    /* Sets texture addressing mode for T texture coordinate. */
    void setTextureAddressingModeT(EGETexture::AddressingMode mode);

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(Texture2D);

    /*! Minifying function filter. */
    EGETexture::Filter m_minFilter;
    /*! Magnification function filter. */
    EGETexture::Filter m_magFilter;
    /*! Texture addressing mode for S texture coordinate. */
    EGETexture::AddressingMode m_addressingModeS;
    /*! Texture addressing mode for T texture coordinate. */
    EGETexture::AddressingMode m_addressingModeT;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_TEXTURE2D_H
