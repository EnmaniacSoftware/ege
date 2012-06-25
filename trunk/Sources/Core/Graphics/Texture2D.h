#ifndef EGE_CORE_TEXTURE2D_H
#define EGE_CORE_TEXTURE2D_H

/** Base object representing 2D texture.
*/

#include <EGE.h>
#include <EGETexture.h>
#include <EGEString.h>
#include <EGEImage.h>
#include "Core/Graphics/Render/RenderTarget.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Texture2D, PTexture2D)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Texture2D : public Object
{
  /* For accessing private data. */
  friend class RendererPrivate;
  
  public:

    Texture2D(Application* app, const String& name);
    virtual ~Texture2D();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /* Creates render texture. */
    static PTexture2D CreateRenderTexture(Application* app, const String& name, s32 width, s32 height, PixelFormat format);

  public:

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /*! Returns name. */
    inline const String& name() const { return m_name; }
    /* Creates texture from given file. */
    EGEResult create(const String& path);
    /* Creates texture from given buffer. */
    EGEResult create(const PDataBuffer& buffer);
    /* Sets minifying function filter. */
    void setMinFilter(EGETexture::Filter filter);
    /* Sets magnification function filter. */
    void setMagFilter(EGETexture::Filter filter);
    /* Sets texture addressing mode for S texture coordinate. */
    void setTextureAddressingModeS(EGETexture::AddressingMode mode);
    /* Sets texture addressing mode for T texture coordinate. */
    void setTextureAddressingModeT(EGETexture::AddressingMode mode);
    /*! Returns render target. */
    inline PRenderTarget renderTarget() const { return m_target; }
    /*! Returns width. */
    inline s32 width() const { return m_width; }
    /*! Returns height. */
    inline s32 height() const { return m_height; }
    /*! Returns pixel format. */
    inline PixelFormat format() const { return m_format; }
    /* Enables/disables compression. */
    void setCompressionEnabled(bool set);
    /*! Returns compression flag. */
    inline bool isCompressionEnabled() const { return m_compression; }

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(Texture2D);

    /*! Texture name. */
    String m_name; 
    /*! Minifying function filter. */
    EGETexture::Filter m_minFilter;
    /*! Magnification function filter. */
    EGETexture::Filter m_magFilter;
    /*! Texture addressing mode for S texture coordinate. */
    EGETexture::AddressingMode m_addressingModeS;
    /*! Texture addressing mode for T texture coordinate. */
    EGETexture::AddressingMode m_addressingModeT;
    /*! Render target. NULL if texture is not targetable. */
    PRenderTarget m_target;
    /*! Texture width. */
    s32 m_width;
    /*! Texture height. */
    s32 m_height;
    /*! Pixel format. */
    PixelFormat m_format;
    /*! Compression flag. */
    bool m_compression;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_TEXTURE2D_H
