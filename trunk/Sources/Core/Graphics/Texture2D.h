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
class IHardwareResourceProvider;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Texture2D : public Object
{
  /* For accessing private data. */
  friend class RenderSystemPrivate;
  
  public:

    virtual ~Texture2D();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Returns TRUE if object is valid. */
    bool isValid() const;
    /*! Returns name. */
    const String& name() const { return m_name; }
    /*! Creates texture from given file. 
     *  @note Calling thread must be able to issue underlying 3D API commands.
     */
    EGEResult create(const String& path);
    /*! Creates texture from given buffer. 
     *  @note Calling thread must be able to issue underlying 3D API commands.
     */
    EGEResult create(const PDataBuffer& buffer);
    /*! Creates texture from given image. 
     *  @note Calling thread must be able to issue underlying 3D API commands.
     */
    EGEResult create(const PImage& image);
    /*! Returns render target. */
    PRenderTarget renderTarget() const { return m_target; }
    /*! Returns width. */
    s32 width() const { return m_width; }
    /*! Returns height. */
    s32 height() const { return m_height; }
    /*! Returns pixel format. */
    PixelFormat format() const { return m_format; }

  protected:

    /*! Constructing only via RenderSystem. */
    Texture2D(Application* app, const String& name, IHardwareResourceProvider* provider);

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(Texture2D);

    /*! Texture name. */
    String m_name; 
    /*! Render target. NULL if texture is not targetable. */
    PRenderTarget m_target;
    /*! Texture width. */
    s32 m_width;
    /*! Texture height. */
    s32 m_height;
    /*! Pixel format. */
    PixelFormat m_format;
    /*! Texture provider used to create texture. */
    IHardwareResourceProvider* m_provider;
    /*! Texture minifying function filter. */
    EGETexture::Filter m_minFilter;
    /*! Texture magnification function filter. */
    EGETexture::Filter m_magFilter;
    /*! Texture addressing mode for S texture coordinate. */
    EGETexture::AddressingMode m_addressingModeS;
    /*! Texture addressing mode for T texture coordinate. */
    EGETexture::AddressingMode m_addressingModeT;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_TEXTURE2D_H
