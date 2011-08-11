#include <EGEOpenGL.h>
#include "Core/Graphics/Image/Image.h"
#include "Core/Graphics/OpenGL/Texture2DOGL.h"
#include "Core/Graphics/OpenGL/RenderTextureCopyOGL.h"
#include "Core/Graphics/OpenGL/RenderTextureFBOOGL.h"
#include <EGEDevice.h>
#include <EGEDebug.h>

EGE_NAMESPACE

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
Texture2DPrivate::Texture2DPrivate(Texture2D* base) : m_d(base), m_id(0), m_internalFormat(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Texture2DPrivate::~Texture2DPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates texture from given file. */
EGEResult Texture2DPrivate::create(const String& path)
{
  EGEResult result = EGE_SUCCESS;

  // load image
  Image image(d_func()->app());
  if (EGE_SUCCESS != (result = image.load(path)))
  {
    // error!
    return result;
  }

  // set texture data
  d_func()->m_width  = image.width();
  d_func()->m_height = image.height();
  d_func()->m_format = image.format();

  // create empty texture
  if (EGE_SUCCESS != (result = create()))
  {
    // error!
    return result;
  }

  // copy pixel data
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, d_func()->width(), d_func()->height(), m_internalFormat, GL_UNSIGNED_BYTE, image.data()->data());

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates texture empty from current data. */
EGEResult Texture2DPrivate::create()
{
  // setup 4 byte alignment
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  // generate and bind OGL texture
  glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mapFilter(d_func()->m_minFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mapFilter(d_func()->m_magFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mapAddressingMode(d_func()->m_addressingModeS));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mapAddressingMode(d_func()->m_addressingModeT));

  //if ( glExt::EXT_texture_filter_anisotropic == true )
  //{
  //  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_fAnisotropyFactor );
  //}

  // get input image and texture format
  GLint internalFormat = 0;
  GLenum imageFormat = 0;
  switch (d_func()->format())
  {
    case EGEImage::RGBA_8888: imageFormat = GL_RGBA; internalFormat = GL_RGBA; break;
    case EGEImage::RGB_888:   imageFormat = GL_RGB; internalFormat = GL_RGB; break;
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

      EGE_ASSERT("Invalid format");
      return EGE_ERROR_NOT_SUPPORTED;
  }

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

  // check if compression enabled
  //if ( m_bCompression == true && glExt::EXT_texture_compression_s3tc == true )
  //{
  //  switch( eFormat )
  //  {
  //    case CTexture::RGB:  eFormat = RGB_DXT1; break;
  //    case CTexture::RGBA: eFormat = RGBA_DXT5; break;
  //  }
  //}

  // create texture
  // NOTE: for compatibility with OpenGLES 'internalFormat' MUST be the same as 'imageFormat'
  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, d_func()->width(), d_func()->height(), 0, imageFormat, GL_UNSIGNED_BYTE, NULL);
 
  // store internal format
  m_internalFormat = internalFormat;

  // check for error
  GLenum error;
  if (GL_NO_ERROR != (error = glGetError()))
  {
    // error!
    return EGE_ERROR;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Maps filter to OpenGL equivalent. */
GLint Texture2DPrivate::mapFilter(EGETexture::Filter filter) const
{
  switch (filter)
  {
    case EGETexture::BILINEAR:          return GL_NEAREST;
    case EGETexture::TRILINEAR:         return GL_LINEAR;
    case EGETexture::MIPMAP_BILINEAR:   return GL_LINEAR_MIPMAP_NEAREST;
    case EGETexture::MIPMAP_TRILINEAR:  return GL_LINEAR_MIPMAP_LINEAR;
  }

  return GL_NEAREST;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Maps texture addressing mode into OpenGL equivalent. */
GLint Texture2DPrivate::mapAddressingMode(EGETexture::AddressingMode mode) const
{
  switch (mode)
  {
#ifndef EGE_RENDERING_OPENGLES_1
    case EGETexture::AM_CLAMP:  return GL_CLAMP;
#else
    case EGETexture::AM_CLAMP:  return GL_CLAMP_TO_EDGE;
#endif
    case EGETexture::AM_REPEAT: return GL_REPEAT;
  }

  return GL_REPEAT;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates associated render target. */
EGEResult Texture2DPrivate::createRenderTarget()
{
  ConfigParams params;
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

  // check if could not be allocated
  if (NULL == d_func()->m_target)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
