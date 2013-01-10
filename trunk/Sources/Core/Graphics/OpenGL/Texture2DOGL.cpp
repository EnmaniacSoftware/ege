#include <EGEOpenGL.h>
#include "Core/Graphics/Image/Image.h"
#include "Core/Graphics/OpenGL/Texture2DOGL.h"
#include "Core/Graphics/OpenGL/RenderTextureCopyOGL.h"
#include "Core/Graphics/OpenGL/RenderTextureFBOOGL.h"
#include <EGEDevice.h>
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Texture2DPrivate)
EGE_DEFINE_DELETE_OPERATORS(Texture2DPrivate)

        /** Returns the closest power-of-two number greater or equal to value.
            @note 0 and 1 are powers of two, so 
                firstPO2From(0)==0 and firstPO2From(1)==1.
        */
        //static u32 firstPO2From(u32 n)
        //{
        //    --n;            
        //    n |= n >> 16;
        //    n |= n >> 8;
        //    n |= n >> 4;
        //    n |= n >> 2;
        //    n |= n >> 1;
        //    ++n;
        //    return n;
        //}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Texture2DPrivate::Texture2DPrivate(Texture2D* base) : m_d(base), 
                                                      m_id(0), 
                                                      m_internalFormat(0),
                                                      m_typeFormat(0)
{
  // generate OGL texture
  glGenTextures(1, &m_id);
  OGL_CHECK();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Texture2DPrivate::~Texture2DPrivate()
{
  egeDebug() << "Destroying texture" << m_id;

  if (0 != m_id)
  {
    glDeleteTextures(1, &m_id);
    m_id = 0;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Texture2DPrivate::isValid() const
{
  return (0 != m_id);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult Texture2DPrivate::create(const String& path)
{
  EGEResult result = EGE_SUCCESS;

  // load image
  PImage image = Image::Load(path);
  if (NULL == image)
  {
    // error!
    return EGE_ERROR;
  }

  // create texture
  if (EGE_SUCCESS != (result = create(image)))
  {
    // error!
    return result;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult Texture2DPrivate::create(const PDataBuffer& buffer)
{
  EGEResult result = EGE_SUCCESS;

  // load image
  PImage image = Image::Load(buffer);
  if (NULL == image)
  {
    // error!
    return EGE_ERROR;
  }

  // create texture
  if (EGE_SUCCESS != (result = create(image)))
  {
    // error!
    return result;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult Texture2DPrivate::create(const PImage& image)
{
  egeDebug() << "Creating texture" << image->width() << image->height() << image->format();

  // set texture data
  d_func()->m_width  = image->width();
  d_func()->m_height = image->height();
  d_func()->m_format = image->format();

  // setup 4 byte alignment
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

//	glBindTexture(GL_TEXTURE_2D, m_id);

  //if ( glExt::EXT_texture_filter_anisotropic == true )
  //{
  //  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_fAnisotropyFactor );
  //}

  // get input image and texture format
  GLint internalFormat  = 0;
  GLenum imageFormat    = 0;
  GLenum type           = GL_UNSIGNED_BYTE;

  switch (d_func()->format())
  {
    case PF_RGBA_8888:        imageFormat = GL_RGBA;  internalFormat = GL_RGBA; type = GL_UNSIGNED_BYTE; break;
    case PF_RGB_888:          imageFormat = GL_RGB;   internalFormat = GL_RGB;  type = GL_UNSIGNED_BYTE; break;
    case PF_RGBA_5551:        imageFormat = GL_RGBA;  internalFormat = GL_RGBA; type = GL_UNSIGNED_SHORT_5_5_5_1; break;
    case PF_RGBA_4444:        imageFormat = GL_RGBA;  internalFormat = GL_RGBA; type = GL_UNSIGNED_SHORT_4_4_4_4; break;
    case PF_RGB_565:          imageFormat = GL_RGB;   internalFormat = GL_RGB;  type = GL_UNSIGNED_SHORT_5_6_5; break;
    case PF_PVRTC_RGB_2BPP:   imageFormat = GL_RGB;   internalFormat = GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG; break;
    case PF_PVRTC_RGB_4BPP:   imageFormat = GL_RGB;   internalFormat = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG; break;
    case PF_PVRTC_RGBA_2BPP:  imageFormat = GL_RGBA;  internalFormat = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG; break;
    case PF_PVRTC_RGBA_4BPP:  imageFormat = GL_RGBA;  internalFormat = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG; break;
    case PF_DXT1:             imageFormat = GL_RGB;   internalFormat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT; break;
    case PF_DXT2:             imageFormat = GL_RGBA;  internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; break;
    case PF_DXT3:             imageFormat = GL_RGBA;  internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; break;
    case PF_DXT5:             imageFormat = GL_RGBA;  internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; break;

    //case CImage::RGB:  eImageFormat = RGB; break;  
    //case CImage::BGR: 
    //  
    //  if ( glExt::EXT_bgra == false )
    //  {
    //    // not supported
    //    return false;
    //  }

    //  eImageFormat = BGR; 
    //  break;

    //case CImage::BGRA: 
    //  
    //  if ( glExt::EXT_bgra == false )
    //  {
    //    // not supported
    //    return false;
    //  }

    //  eImageFormat = BGRA; 
    //  break;
    
    default:

      EGE_ASSERT(false && "Invalid format");
      return EGE_ERROR_NOT_SUPPORTED;
  }
 // }
  //else
  //{
  //  // S3TC compression
  //  if (Device::HasRenderCapability(EGEDevice::RENDER_CAPS_TEXTURE_COMPRESSION_S3TC))
  //  {
  //    switch (d_func()->format())
  //    {
  //      case PF_RGB_888:   imageFormat = GL_RGB; internalFormat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT; break;
  //      case PF_RGBA_8888: imageFormat = GL_RGBA; internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; break;
  //    }
  //  }
  //  // PVRTC
  //  else if (Device::HasRenderCapability(EGEDevice::RENDER_CAPS_TEXTURE_COMPRESSION_PVRTC))
  //  {
  //    switch (d_func()->format())
  //    {
  //      case PF_PVRTC_RGB_2BPP:  imageFormat = GL_RGB; internalFormat = GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG; break;
  //      case PF_PVRTC_RGB_4BPP:  imageFormat = GL_RGB; internalFormat = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG; break;
  //      case PF_PVRTC_RGBA_2BPP: imageFormat = GL_RGBA; internalFormat = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG; break;
  //      case PF_PVRTC_RGBA_4BPP: imageFormat = GL_RGBA; internalFormat = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG; break;
  //    }
  //  }
  //}

  // determine texture format
  //if ( eFormat == CTexture::UNKNOWN )
  //{
  //  // determine based on image format
  //  switch( eImageFormat )
  //  {
  //    case CTexture::RGB:
  //    case CTexture::BGR:

  //      eFormat = CTexture::RGB;
  //      break;

  //    case CTexture::RGBA:
  //    case CTexture::BGRA:

  //      eFormat = CTexture::RGBA;
  //      break;
  //  }
  //}

  // check if mipmapping enabled
  //if ( m_bMipMapping == true )
  //{
  //  // check if SGIS extension supported
  //  if ( glExt::SGIS_generate_mipmap == true )
  //  {
  //    glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
  //  }
  //  else
  //  {
  //    assert( "implement mipmapping via gluBuild2DMipmaps and make sure glTexImage2D is not called!!!" );
  //  }
  //}

  // check if compressed image data
  if (image->isCompressed())
  {
    // create compressed texture
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image->width(), image->height(), 0, static_cast<GLsizei>(image->data()->size()), 
                           image->data()->data());
  }
  else
  {
    // create texture
    // NOTE: for compatibility with OpenGLES 'internalFormat' MUST be the same as 'imageFormat'
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, d_func()->width(), d_func()->height(), 0, imageFormat, type, image->data() ? image->data()->data() : NULL);
  }

  // store data
  m_internalFormat = internalFormat;
  m_typeFormat     = type;

  egeDebug() << "Creating texture done" << m_id;

  // check for error
  OGL_CHECK();

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult Texture2DPrivate::createRenderTarget()
{
  egeWarning() << "Creating render target!";

  Dictionary params;
  params[EGE_RENDER_TARGET_PARAM_NAME]    = d_func()->name();
  params[EGE_RENDER_TARGET_PARAM_WIDTH]   = String::FromNumber(d_func()->width());
  params[EGE_RENDER_TARGET_PARAM_HEIGHT]  = String::FromNumber(d_func()->height());

  // check if Frame Buffer Object is supported
  if (Device::HasRenderCapability(EGEDevice::RENDER_CAPS_FBO))
  {
    d_func()->m_target = ege_new RenderTextureFBOOGL(d_func()->app(), params, GL_TEXTURE_2D, GL_TEXTURE_2D, id());
  }
  else
  {
    d_func()->m_target = ege_new RenderTextureCopyOGL(d_func()->app(), params, GL_TEXTURE_2D, GL_TEXTURE_2D, id());
  }

  egeWarning() << "Creating render target done" << d_func()->m_target;

  // check if could not be allocated
  if (NULL == d_func()->m_target)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END