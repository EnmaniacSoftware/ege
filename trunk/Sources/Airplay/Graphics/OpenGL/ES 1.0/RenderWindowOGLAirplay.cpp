#include "Core/Application/Application.h"
#include "Airplay/Graphics/OpenGL/ES 1.0/RenderWindowOGLAirplay.h"
#include <s3e.h>
#include <EGEMath.h>
#include <EGEDevice.h>
#include <GLES/gl.h>
#include <GLES/egl.h>
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderWindowOGLAirplay::RenderWindowOGLAirplay(Application* app, const Dictionary& params) : RenderWindow(app, params), 
                                                                                             m_eglDisplay(EGL_NO_DISPLAY), 
                                                                                             m_eglContext(EGL_NO_CONTEXT),
                                                                                             m_eglSurface(EGL_NO_SURFACE)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderWindowOGLAirplay::~RenderWindowOGLAirplay()
{
  destroy();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult RenderWindowOGLAirplay::construct(const Dictionary& params)
{
  bool error = false;

  // register for surface orientation changes
  if (S3E_RESULT_SUCCESS != s3eSurfaceRegister(S3E_SURFACE_SCREENSIZE, RenderWindowOGLAirplay::OrientationChangeCB, this))
  {
    // error!
    egeWarning() << "Count not register surface orientation change callback.";
  }

  // decompose param list
  Dictionary::const_iterator iterColorBits = params.find(EGE_RENDER_TARGET_PARAM_COLOR_BITS);
  Dictionary::const_iterator iterDepthBits = params.find(EGE_RENDER_WINDOW_PARAM_DEPTH_BITS);
  Dictionary::const_iterator iterLandscape = params.find(EGE_ENGINE_PARAM_LANDSCAPE_MODE);

  // check if required parameters are NOT present
  if (iterColorBits == params.end() || iterDepthBits == params.end())
  {
    // error!
    return EGE_ERROR;
  }

  bool landscape = (iterLandscape != params.end()) ? iterLandscape->second.toBool(&error) : false;

  // set default paramerter values
  s32 colorBits = iterColorBits->second.toInt(&error);
  s32 depthBits = iterDepthBits->second.toInt(&error);

  if (error)
  {
    // error!
    return EGE_ERROR;
  }

  // get device screen dimensions
  m_physicalWidth  = Device::SurfaceWidth();
  m_physicalHeight = Device::SurfaceHeight();

  // get current orientation screen dimensions
  m_width  = s3eSurfaceGetInt(S3E_SURFACE_WIDTH);
  m_height = s3eSurfaceGetInt(S3E_SURFACE_HEIGHT);

  // determine initial screen orientation
  switch (s3eSurfaceGetInt(S3E_SURFACE_DEVICE_BLIT_DIRECTION))
  {
    case S3E_SURFACE_BLIT_DIR_NORMAL: m_orientationRotation.fromDegrees(0.0f); break;
    case S3E_SURFACE_BLIT_DIR_ROT90:  m_orientationRotation.fromDegrees(90.0f); break;
    case S3E_SURFACE_BLIT_DIR_ROT180: m_orientationRotation.fromDegrees(180.0f); break;
    case S3E_SURFACE_BLIT_DIR_ROT270: m_orientationRotation.fromDegrees(270.0f); break;

    default:

      egeWarning() << "Unknown blit direction!";
      break;
  }

  //EGE_PRINT("Pwidth: %d Pheight: %d width: %d height: %d orient: %f", m_physicalWidth, m_physicalHeight, m_width, m_height, m_orientationRotation.degrees());

  // apply dimensions according to landscape requirement
  //if (landscape)
  //{
  //  m_height = Math::Min(m_physicalWidth, m_physicalHeight);
  //  m_width  = Math::Max(m_physicalWidth, m_physicalHeight);

  //  // check if rotation from portrait to landscape is needed for render target
  //  if (m_physicalWidth != m_width)
  //  {
  //    m_orientationRotation.fromDegrees(-90);
  //  }
  //}
  //else
  //{
  //  m_height = Math::Max(m_physicalWidth, m_physicalHeight);
  //  m_width  = Math::Min(m_physicalWidth, m_physicalHeight);

  //  // check if rotation from landscape to portrait is needed for render target
  //  if (m_physicalWidth != m_width)
  //  {
  //    m_orientationRotation.fromDegrees(90);
  //  }
  //}

  static const EGLint configAttribs[] =
  {
    EGL_RED_SIZE,       colorBits / 3,
    EGL_GREEN_SIZE,     colorBits / 3,
    EGL_BLUE_SIZE,      colorBits / 3,
    EGL_DEPTH_SIZE,     depthBits,
    EGL_ALPHA_SIZE,     EGL_DONT_CARE,
    EGL_STENCIL_SIZE,   EGL_DONT_CARE,
    EGL_SURFACE_TYPE,   EGL_WINDOW_BIT,
    EGL_NONE
  };

  EGLBoolean success;

  EGLint numConfigs;
  EGLint majorVersion;
  EGLint minorVersion;

  EGLConfig sEglConfig;

  m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);

  success = eglInitialize(m_eglDisplay, &majorVersion, &minorVersion);
  if (EGL_FALSE != success)
  {
    success = eglGetConfigs(m_eglDisplay, NULL, 0, &numConfigs);
  }

  if (EGL_FALSE != success)
  {
    success = eglChooseConfig(m_eglDisplay, configAttribs, &sEglConfig, 1, &numConfigs);
  }

  if (EGL_FALSE != success)
  {
    m_eglSurface = eglCreateWindowSurface(m_eglDisplay, sEglConfig, s3eGLGetNativeWindow(), NULL);
    if (m_eglSurface == EGL_NO_SURFACE)
    {
      success = EGL_FALSE;
    }
  }
  
  if (EGL_FALSE != success)
  {
    m_eglContext = eglCreateContext(m_eglDisplay, sEglConfig, NULL, NULL);
    if (m_eglContext == EGL_NO_CONTEXT)
    {
      success = EGL_FALSE;
    }
  }

  if (EGL_FALSE != success)
  {
    success = eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);
  }

  // obtain a detailed description of surface pixel format
  if (EGL_FALSE != success)
  {
    // supply surface data for for device info

    EGLint value;

    if (EGL_TRUE == eglGetConfigAttrib(m_eglDisplay, sEglConfig, EGL_RED_SIZE, &value))
    {
      Device::SetSurfaceRedChannelBitsCount(static_cast<u32>(value));
    }
    if (EGL_TRUE == eglGetConfigAttrib(m_eglDisplay, sEglConfig, EGL_GREEN_SIZE, &value))
    {
      Device::SetSurfaceGreenChannelBitsCount(static_cast<u32>(value));
    }
    if (EGL_TRUE == eglGetConfigAttrib(m_eglDisplay, sEglConfig, EGL_BLUE_SIZE, &value))
    {
      Device::SetSurfaceBlueChannelBitsCount(static_cast<u32>(value));
    }
    if (EGL_TRUE == eglGetConfigAttrib(m_eglDisplay, sEglConfig, EGL_ALPHA_SIZE, &value))
    {
      Device::SetSurfaceAlphaChannelBitsCount(static_cast<u32>(value));
    }
  }

  if (EGL_FALSE == success)
  {
    destroy();
    return EGE_ERROR;
  }

  // detect capabilities
  detectCapabilities();

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderWindowOGLAirplay::destroy()
{
  // unregister surface orientation change callback
  s3eSurfaceUnRegister(S3E_SURFACE_SCREENSIZE, RenderWindowOGLAirplay::OrientationChangeCB);

  eglMakeCurrent(m_eglDisplay, NULL, NULL, NULL);

  eglDestroyContext(m_eglDisplay, m_eglContext);
  eglDestroySurface(m_eglDisplay, m_eglSurface);
  eglTerminate(m_eglDisplay);

  m_eglDisplay = EGL_NO_DISPLAY;
  m_eglContext = EGL_NO_CONTEXT;
  m_eglSurface = EGL_NO_SURFACE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderWindowOGLAirplay::showFrameBuffer()
{
  eglSwapBuffers(m_eglDisplay, m_eglSurface);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool RenderWindowOGLAirplay::isValid() const
{
  if ( ! RenderWindow::isValid())
  {
    // not valid
    return false;
  }

  return (EGL_NO_SURFACE != m_eglSurface) && (EGL_NO_CONTEXT != m_eglContext) && (EGL_NO_DISPLAY != m_eglDisplay);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult RenderWindowOGLAirplay::enableFullScreen(s32 width, s32 height, bool enable)
{
  EGE_UNUSED(width);
  EGE_UNUSED(height);
  EGE_UNUSED(enable);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool RenderWindowOGLAirplay::requiresTextureFlipping() const
{
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
int32 RenderWindowOGLAirplay::OrientationChangeCB(void* systemData, void* userData)
{
  s3eSurfaceOrientation* so  = reinterpret_cast<s3eSurfaceOrientation*>(systemData);
  RenderWindowOGLAirplay* me = reinterpret_cast<RenderWindowOGLAirplay*>(userData);

  // process accroding to blit direction
  switch (so->m_DeviceBlitDirection)
  {
    case S3E_SURFACE_BLIT_DIR_NORMAL: me->m_orientationRotation.fromDegrees(0.0f); break;
    case S3E_SURFACE_BLIT_DIR_ROT90:  me->m_orientationRotation.fromDegrees(90.0f); break;
    case S3E_SURFACE_BLIT_DIR_ROT180: me->m_orientationRotation.fromDegrees(180.0f); break;
    case S3E_SURFACE_BLIT_DIR_ROT270: me->m_orientationRotation.fromDegrees(270.0f); break;

    default:

      egeWarning() << "Unknown blit direction!";
      break;
  }

  int32 bd = s3eSurfaceGetInt(S3E_SURFACE_BLIT_DIRECTION);
  int32 dbd = s3eSurfaceGetInt(S3E_SURFACE_DEVICE_BLIT_DIRECTION);

//  EGE_PRINT("ORIENTATION %d %f   bd: %d dbd: %d w: %d h: %d", so->m_DeviceBlitDirection, me->m_orientationRotation.degrees(), bd, dbd, s3eSurfaceGetInt(S3E_SURFACE_WIDTH), s3eSurfaceGetInt(S3E_SURFACE_HEIGHT));

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderWindowOGLAirplay::detectCapabilities()
{
  // get list of all extensions
  String extensionString(reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS)));
  StringArray extensionArray = extensionString.split(" ");

  for (StringArray::const_iterator it = extensionArray.begin(); it != extensionArray.end(); ++it)
  {
    egeDebug() << "Available OGL extension:" << *it;
  }

	GLint value;

  // get number of texture units
	glGetIntegerv(GL_MAX_TEXTURE_UNITS, &value);
  Device::SetTextureUnitsCount(static_cast<u32>(value));

  // detect maximal texture size
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value);
  Device::SetTextureMaxSize(static_cast<u32>(value));

  if (extensionArray.contains("GL_APPLE_texture_2D_limited_npot"))
  {
    Device::SetRenderCapability(EGEDevice::RENDER_CAPS_APPLE_LIMITED_NPOT_TEXTURE, true);
  }

  // multitexturing is supported by default
  Device::SetRenderCapability(EGEDevice::RENDER_CAPS_MULTITEXTURE, true);

  // check if frame buffer object is supported
  if (extensionArray.contains("GL_OES_framebuffer_object"))
  {
    glBindFramebuffer         = reinterpret_cast<PFNGLBINDFRAMEBUFFEROESPROC>(eglGetProcAddress("glBindFramebufferOES"));
    glDeleteFramebuffers      = reinterpret_cast<PFNGLDELETEFRAMEBUFFERSOESPROC>(eglGetProcAddress("glDeleteFramebuffersOES"));
    glGenFramebuffers         = reinterpret_cast<PFNGLGENFRAMEBUFFERSOESPROC>(eglGetProcAddress("glGenFramebuffersOES"));
    glCheckFramebufferStatus  = reinterpret_cast<PFNGLCHECKFRAMEBUFFERSTATUSOESPROC>(eglGetProcAddress("glCheckFramebufferStatusOES"));
    glFramebufferTexture2D    = reinterpret_cast<PFNGLFRAMEBUFFERTEXTURE2DOESPROC>(eglGetProcAddress("glFramebufferTexture2DOES"));

    if ((NULL != glBindFramebuffer) && (NULL != glDeleteFramebuffers) && (NULL != glGenFramebuffers) && 
        (NULL != glCheckFramebufferStatus) && (NULL != glFramebufferTexture2D))
    {
      Device::SetRenderCapability(EGEDevice::RENDER_CAPS_FBO, true);
    }
  }

  // check if VBO mapping is supported
  if (extensionArray.contains("GL_OES_mapbuffer"))
  {
    glMapBuffer   = reinterpret_cast<PFNGLMAPBUFFEROESPROC>(eglGetProcAddress("glMapBufferOES"));
    glUnmapBuffer = reinterpret_cast<PFNGLUNMAPBUFFEROESPROC>(eglGetProcAddress("glUnmapBufferOES"));

    if ((NULL != glMapBuffer) && (NULL != glUnmapBuffer))
    {
      Device::SetRenderCapability(EGEDevice::RENDER_CAPS_MAP_BUFFER, true);
    }
  }

  // check if min and max blending functions are supported
  //if (extensionArray.contains("GL_ARB_imaging") || extensionArray.contains("GL_EXT_blend_minmax"))
  //{
  //  glBlendEquation = (PFNGLBLENDEQUATIONPROC) wglGetProcAddress("glBlendEquationARB");
  //  if (!glBlendEquation)
  //  {
  //    glBlendEquation = (PFNGLBLENDEQUATIONPROC) wglGetProcAddress("glBlendEquationEXT");
  //  }

  //  if (glBlendEquation)
  //  {
  //    Device::SetRenderCapability(EGEDevice::RENDER_CAPS_BLEND_MINMAX, true);
  //  }
  //}
  
  // Combine texture environment mode is supported by default
  Device::SetRenderCapability(EGEDevice::RENDER_CAPS_COMBINE_TEXTURE_ENV, true);

  // Point sprite is supported by defult
  Device::SetRenderCapability(EGEDevice::RENDER_CAPS_POINT_SPRITE, true);

  // Point sprite size array is supported by default
  Device::SetRenderCapability(EGEDevice::RENDER_CAPS_POINT_SPRITE_SIZE, true);

  // Vertex Buffer Objects is supported by default
  Device::SetRenderCapability(EGEDevice::RENDER_CAPS_VBO, true);

  // check for texture compressions support
  if (extensionArray.contains("GL_IMG_texture_compression_pvrtc"))
  {
    Device::SetRenderCapability(EGEDevice::RENDER_CAPS_TEXTURE_COMPRESSION_PVRTC, true);
  }

  if (extensionArray.contains("GL_EXT_texture_compression_s3tc"))
  {
    Device::SetRenderCapability(EGEDevice::RENDER_CAPS_TEXTURE_COMPRESSION_S3TC, true);
  }

  // check for Uint32 element indicies support
  if (extensionArray.contains("GL_OES_element_index_uint"))
  {
    Device::SetRenderCapability(EGEDevice::RENDER_CAPS_ELEMENT_INDEX_UINT, true);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END