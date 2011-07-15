#include "Core/Graphics/Image.h"
#include "Core/Data/DataBuffer.h"
#include <EGETexture.h>
#include <EGEDebug.h>

#ifdef EGE_PLATFORM_WIN32
#include <gl/GL.h>
#else
#include "GLES/gl.h"
#endif

#if EGE_RENDERING_OPENGL_2 || EGE_RENDERING_OPENGLES_1
#include "Core/Graphics/OpenGL/Texture2DOGL.h"
#endif // EGE_RENDERING_OPENGL_2 || EGE_RENDERING_OPENGLES_1

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(Texture2D)
EGE_DEFINE_DELETE_OPERATORS(Texture2D)

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
Texture2D::Texture2D(Application* app, EGETexture::Filter minFilter, EGETexture::Filter magFilter, EGETexture::Wrap wrapS, EGETexture::Wrap wrapT) 
: Object(app, EGE_OBJECT_UID_TEXTURE_2D), m_id(0), m_minFilter(minFilter), m_magFilter(magFilter), m_wrapS(wrapS), m_wrapT(wrapT)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Texture2D::~Texture2D()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult Texture2D::create(const String& path)
{
  EGEResult result = EGE_SUCCESS;

  // load image
  Image image(app());
  if (EGE_SUCCESS != (result = image.load(path)))
  {
    // error!
    return result;
  }

  // setup 4 byte alignment
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  // generate and bind OGL texture
  glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

  //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mapFilter(m_minFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mapFilter(m_magFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mapWrap(m_wrapS));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mapWrap(m_wrapT));

  //if ( glExt::EXT_texture_filter_anisotropic == true )
  //{
  //  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_fAnisotropyFactor );
  //}

  // get input image and texture format
  GLint internalFormat = 0;
  GLint imageFormat = 0;
  switch (image.format())
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

      EGE_ASSERT(false);
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
  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image.width(), image.height(), 0, imageFormat, GL_UNSIGNED_BYTE, image.data()->data());

  // check for error
  GLenum error;
  if (GL_NO_ERROR != (error = glGetError()))
  {
    // error!
    return EGE_ERROR;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Maps filter to OpenGL equivalent. */
GLint Texture2D::mapFilter(EGETexture::Filter filter) const
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
/*! Maps wrap parameter to OpenGL equivalent. */
GLint Texture2D::mapWrap(EGETexture::Wrap wrap) const
{
  switch (wrap)
  {
#ifdef EGE_PLATFORM_WIN32
    case EGETexture::CLAMP:  return GL_CLAMP;
#else
    case EGETexture::CLAMP:  return GL_CLAMP_TO_EDGE;
#endif
    case EGETexture::REPEAT: return GL_REPEAT;
  }

  return GL_REPEAT;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets minifying function filter. */
void Texture2D::setMinFilter(EGETexture::Filter filter)
{
  m_minFilter = filter;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Sets magnification function filter. */
void Texture2D::setMagFilter(EGETexture::Filter filter)
{
  m_magFilter = filter;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Sets wrap parameter for S coordinate. */
void Texture2D::setWrapS(EGETexture::Wrap mode)
{
  m_wrapS = mode;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Sets wrap parameter for T coordinate. */
void Texture2D::setWrapT(EGETexture::Wrap mode)
{
  m_wrapT = mode;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
