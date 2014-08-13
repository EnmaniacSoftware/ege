#ifndef EGE_CORE_GRAPHICS_TEXTURE2D_H
#define EGE_CORE_GRAPHICS_TEXTURE2D_H

/** Base object representing 2D texture.
*/

#include "EGE.h"
#include "EGETexture.h"
#include "EGEString.h"
#include "EGEImage.h"
#include "Core/Graphics/Render/RenderTarget.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Available texture filters. */
enum TextureFilter
{
  TF_NEAREST = 0,       /*< for MIN and MAG. Can be used for MipMapping. */
  TF_BILINEAR,          /*< for MIN and MAG. Can be used for MipMapping. */
  TF_TRILINEAR,         /*< for MIN and MipMapping only. */
};
  
/*! Available texture addressing modes. */
enum TextureAddressingMode
{
  AM_CLAMP  = 0,        /*< Texture clapms at values over 1.0. */
  AM_REPEAT             /*< Texture repeats at values over 1.0. */
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Texture2D, PTexture2D)
class IHardwareResourceProvider;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Texture2D : public Object
{
  public:

    virtual ~Texture2D();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Creates texture from given file. 
     *  @param  path  Path to file containing data for texture.
     *  @note Calling thread must be able to issue underlying 3D API commands.
     */
    virtual EGEResult create(const String& path) = 0;
    /*! Creates texture from given buffer.
     *  @param  buffer  Buffer containing texture data.
     *  @note Calling thread must be able to issue underlying 3D API commands.
     */
    virtual EGEResult create(const PDataBuffer& buffer) = 0;
    /*! Creates texture from given image. 
     *  @param  image Image from which texture is to be created.
     *  @note Calling thread must be able to issue underlying 3D API commands.
     */
    virtual EGEResult create(const PImage& image) = 0;

    /*! Returns name. */
    const String& name() const;
    /*! Returns render target. */
    PRenderTarget renderTarget() const;
    /*! Sets render target. */
    void setRenderTarget(PRenderTarget& target);
    /*! Returns width. */
    s32 width() const;
    /*! Returns height. */
    s32 height() const;
    /*! Returns pixel format. */
    PixelFormat format() const;

  protected:

    /*! Constructing only via RenderSystem. */
    Texture2D(Engine& engine, const String& name, IHardwareResourceProvider* provider);
    /*! Returns engine object. */
    Engine& engine() const;

  protected:

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

  private:

    /*! Reference to engine. */
    Engine& m_engine;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_TEXTURE2D_H
