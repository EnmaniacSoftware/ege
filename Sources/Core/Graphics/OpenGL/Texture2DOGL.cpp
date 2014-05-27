#include "Core/Graphics/OpenGL/Texture2DOGL.h"
#include "Core/Graphics/Image/Image.h"
#include "EGEDevice.h"
#include "EGEDebug.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Texture2DOGL::Texture2DOGL(Application* app, const String& name, IHardwareResourceProvider* provider) : Texture2D(app, name, provider) 
                                                                                                      , m_id(0)
                                                                                                      , m_internalFormat(0)
                                                                                                      , m_typeFormat(0)
{
  glGenTextures(1, &m_id);
  OGL_CHECK()
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Texture2DOGL::~Texture2DOGL()
{
  if (0 != m_id)
  {
    glDeleteTextures(1, &m_id);
    OGL_CHECK()
   
    m_id = 0;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult Texture2DOGL::create(const String& path)
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
EGEResult Texture2DOGL::create(const PDataBuffer& buffer)
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
EGEResult Texture2DOGL::create(const PImage& image)
{
  egeDebug(KOpenGLDebugName) << "Creating texture" << image->width() << image->height() << image->format();

  // set texture data
  m_width  = image->width();
  m_height = image->height();
  m_format = image->format();

//	glBindTexture(GL_TEXTURE_2D, m_id);

  //if ( glExt::EXT_texture_filter_anisotropic == true )
  //{
  //  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_fAnisotropyFactor );
  //}

  // get input image and texture format
  GLint internalFormat  = 0;
  GLenum imageFormat    = 0;
  GLenum type           = GL_UNSIGNED_BYTE;

  switch (format())
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
    case PF_DXT1:             imageFormat = GL_RGB;   internalFormat = GL_COMPRESSED_RGB_S3TC_DXT1; break;
    case PF_DXT2:             imageFormat = GL_RGBA;  internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1; break;
    case PF_DXT3:             imageFormat = GL_RGBA;  internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3; break;
    case PF_DXT5:             imageFormat = GL_RGBA;  internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5; break;

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

      EGE_ASSERT_X(false, "Invalid format");
      return EGE_ERROR_NOT_SUPPORTED;
  }
 // }
  //else
  //{
  //  // S3TC compression
  //  if (Device::HasRenderCapability(ERenderCapabilityTextureCompressionS3TC))
  //  {
  //    switch (d_func()->format())
  //    {
  //      case PF_RGB_888:   imageFormat = GL_RGB; internalFormat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT; break;
  //      case PF_RGBA_8888: imageFormat = GL_RGBA; internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; break;
  //    }
  //  }
  //  // PVRTC
  //  else if (Device::HasRenderCapability(ERenderCapabilityTextureCompressionPVRTC))
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
    OGL_CHECK()
  }
  else
  {
    // create texture
    // NOTE: for compatibility with OpenGLES 'internalFormat' MUST be the same as 'imageFormat'
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width(), height(), 0, imageFormat, type, image->data() ? image->data()->data() : NULL);
    OGL_CHECK()
  }

  // store data
  m_internalFormat = internalFormat;
  m_typeFormat     = type;

  egeDebug(KOpenGLDebugName) << "Creating texture done" << m_id;

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
GLuint Texture2DOGL::id() const 
{ 
  return m_id; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
