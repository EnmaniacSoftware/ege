#include "iOS/Graphics/OpenGL/RenderWindowOGLIOS.h"
#include "EGEApplication.h"
#include "EGEMath.h"
#include "EGEDevice.h"
#include "EGEDebug.h"
#import <UIKit/UIWindow.h>
#import <UIKit/UIScreen.h>
#import "iOS/Graphics/OpenGL/ViewOGL.h"

#if EGE_RENDER_OPENGL_FIXED
  #import <OpenGLES/ES1/glext.h>
#endif // EGE_RENDER_OPENGL_FIXED

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderWindowOGLIOS::RenderWindowOGLIOS(Application* app, const Dictionary& params) : RenderWindow(app, params),
                                                                                     m_view(NULL),
                                                                                     m_window(NULL),
                                                                                     m_EAGLContext(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderWindowOGLIOS::~RenderWindowOGLIOS()
{
  destroy();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult RenderWindowOGLIOS::construct(const Dictionary& params)
{
//  bool error = false;

  // get screen size
  CGRect screenBounds = [[UIScreen mainScreen] bounds];
  
  // create full screen view
  m_view = [[OGLView alloc] initWithFrame: screenBounds];
  if (NULL == m_view)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }
  
  // create window
  m_window = [[UIWindow alloc] initWithFrame: screenBounds];
  if (NULL == m_window)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }
  
  // attach view to window
  m_window.backgroundColor = [UIColor whiteColor];
  [m_window addSubview: m_view];
  
  // create rendering context
  EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES1;
  m_EAGLContext = [[EAGLContext alloc] initWithAPI: api];
  if (NULL == m_EAGLContext)
  {
    // error!
    return EGE_ERROR;
  }
  
  // make it current
  if (NO == [EAGLContext setCurrentContext: m_EAGLContext])
  {
    // error!
    return EGE_ERROR;
  }
  
  // detect capabilities
  detectCapabilities();
  
//  glGenRenderbuffers(1, &mColorRenderBuffer);
//  glBindRenderbuffer(GL_RENDERBUFFER, mColorRenderBuffer);
//  [mEAGLContext renderbufferStorage: GL_RENDERBUFFER fromDrawable: mEAGLLayer];
  
  // make it visible
  [m_window makeKeyAndVisible];
  
  // register for surface orientation changes
//  if (S3E_RESULT_SUCCESS != s3eSurfaceRegister(S3E_SURFACE_SCREENSIZE, RenderWindowOGLAirplay::OrientationChangeCB, this))
//  {
//    // error!
//    egeWarning() << "Count not register surface orientation change callback.";
//  }

  // get device screen dimensions
  m_physicalWidth  = Device::SurfaceWidth();
  m_physicalHeight = Device::SurfaceHeight();

  // get current orientation screen dimensions
//  m_width  = s3eSurfaceGetInt(S3E_SURFACE_WIDTH);
//  m_height = s3eSurfaceGetInt(S3E_SURFACE_HEIGHT);

  // determine initial screen orientation
//  switch (s3eSurfaceGetInt(S3E_SURFACE_DEVICE_BLIT_DIRECTION))
//  {
//    case S3E_SURFACE_BLIT_DIR_NORMAL: m_orientationRotation.fromDegrees(0.0f); break;
//    case S3E_SURFACE_BLIT_DIR_ROT90:  m_orientationRotation.fromDegrees(90.0f); break;
//    case S3E_SURFACE_BLIT_DIR_ROT180: m_orientationRotation.fromDegrees(180.0f); break;
//    case S3E_SURFACE_BLIT_DIR_ROT270: m_orientationRotation.fromDegrees(270.0f); break;
//
//    default:
//
//      egeWarning() << "Unknown blit direction!";
//      break;
//  }

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

//  static const EGLint configAttribs[] =
//  {
//    EGL_RED_SIZE,       colorBits / 3,
//    EGL_GREEN_SIZE,     colorBits / 3,
//    EGL_BLUE_SIZE,      colorBits / 3,
//    EGL_DEPTH_SIZE,     depthBits,
//    EGL_ALPHA_SIZE,     EGL_DONT_CARE,
//    EGL_STENCIL_SIZE,   EGL_DONT_CARE,
//    EGL_SURFACE_TYPE,   EGL_WINDOW_BIT,
//    EGL_NONE
//  };
//
//  EGLBoolean success;
//
//  EGLint numConfigs;
//  EGLint majorVersion;
//  EGLint minorVersion;
//
//  EGLConfig sEglConfig;
//
//  m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
//
//  success = eglInitialize(m_eglDisplay, &majorVersion, &minorVersion);
//  if (EGL_FALSE != success)
//  {
//    success = eglGetConfigs(m_eglDisplay, NULL, 0, &numConfigs);
//  }
//
//  if (EGL_FALSE != success)
//  {
//    success = eglChooseConfig(m_eglDisplay, configAttribs, &sEglConfig, 1, &numConfigs);
//  }
//
//  if (EGL_FALSE != success)
//  {
//    m_eglSurface = eglCreateWindowSurface(m_eglDisplay, sEglConfig, s3eGLGetNativeWindow(), NULL);
//    if (m_eglSurface == EGL_NO_SURFACE)
//    {
//      success = EGL_FALSE;
//    }
//  }
//  
//  if (EGL_FALSE != success)
//  {
//    m_eglContext = eglCreateContext(m_eglDisplay, sEglConfig, NULL, NULL);
//    if (m_eglContext == EGL_NO_CONTEXT)
//    {
//      success = EGL_FALSE;
//    }
//  }
//
//  if (EGL_FALSE != success)
//  {
//    success = eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);
//  }
//
//  // obtain a detailed description of surface pixel format
//  if (EGL_FALSE != success)
//  {
//    // supply surface data for for device info
//
//    EGLint value;
//
//    if (EGL_TRUE == eglGetConfigAttrib(m_eglDisplay, sEglConfig, EGL_RED_SIZE, &value))
//    {
//      Device::SetSurfaceRedChannelBitsCount(static_cast<u32>(value));
//    }
//    if (EGL_TRUE == eglGetConfigAttrib(m_eglDisplay, sEglConfig, EGL_GREEN_SIZE, &value))
//    {
//      Device::SetSurfaceGreenChannelBitsCount(static_cast<u32>(value));
//    }
//    if (EGL_TRUE == eglGetConfigAttrib(m_eglDisplay, sEglConfig, EGL_BLUE_SIZE, &value))
//    {
//      Device::SetSurfaceBlueChannelBitsCount(static_cast<u32>(value));
//    }
//    if (EGL_TRUE == eglGetConfigAttrib(m_eglDisplay, sEglConfig, EGL_ALPHA_SIZE, &value))
//    {
//      Device::SetSurfaceAlphaChannelBitsCount(static_cast<u32>(value));
//    }
//  }
//
//  if (EGL_FALSE == success)
//  {
//    destroy();
//    return EGE_ERROR;
//  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderWindowOGLIOS::destroy()
{
  [m_EAGLContext release];
  [m_view release];
  [m_window release];
  
  m_view        = nil;
  m_window      = nil;
  m_EAGLContext = nil;
  
  // unregister surface orientation change callback
//  s3eSurfaceUnRegister(S3E_SURFACE_SCREENSIZE, RenderWindowOGLAirplay::OrientationChangeCB);
//
//  eglMakeCurrent(m_eglDisplay, NULL, NULL, NULL);
//
//  eglDestroyContext(m_eglDisplay, m_eglContext);
//  eglDestroySurface(m_eglDisplay, m_eglSurface);
//  eglTerminate(m_eglDisplay);
//
//  m_eglDisplay = EGL_NO_DISPLAY;
//  m_eglContext = EGL_NO_CONTEXT;
//  m_eglSurface = EGL_NO_SURFACE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderWindowOGLIOS::showFrameBuffer()
{
  //eglSwapBuffers(m_eglDisplay, m_eglSurface);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool RenderWindowOGLIOS::isValid() const
{
  if ( ! RenderWindow::isValid())
  {
    // not valid
    return false;
  }

  return (nil != m_view) && (nil != m_window) && (nil != m_EAGLContext);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult RenderWindowOGLIOS::enableFullScreen(s32 width, s32 height, bool enable)
{
  EGE_UNUSED(width);
  EGE_UNUSED(height);
  EGE_UNUSED(enable);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool RenderWindowOGLIOS::requiresTextureFlipping() const
{
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//int32 RenderWindowOGLIOS::OrientationChangeCB(void* systemData, void* userData)
//{
//  s3eSurfaceOrientation* so  = reinterpret_cast<s3eSurfaceOrientation*>(systemData);
//  RenderWindowOGLAirplay* me = reinterpret_cast<RenderWindowOGLAirplay*>(userData);
//
//  // process accroding to blit direction
//  switch (so->m_DeviceBlitDirection)
//  {
//    case S3E_SURFACE_BLIT_DIR_NORMAL: me->m_orientationRotation.fromDegrees(0.0f); break;
//    case S3E_SURFACE_BLIT_DIR_ROT90:  me->m_orientationRotation.fromDegrees(90.0f); break;
//    case S3E_SURFACE_BLIT_DIR_ROT180: me->m_orientationRotation.fromDegrees(180.0f); break;
//    case S3E_SURFACE_BLIT_DIR_ROT270: me->m_orientationRotation.fromDegrees(270.0f); break;
//
//    default:
//
//      egeWarning() << "Unknown blit direction!";
//      break;
//  }
//
//  int32 bd = s3eSurfaceGetInt(S3E_SURFACE_BLIT_DIRECTION);
//  int32 dbd = s3eSurfaceGetInt(S3E_SURFACE_DEVICE_BLIT_DIRECTION);
//
////  EGE_PRINT("ORIENTATION %d %f   bd: %d dbd: %d w: %d h: %d", so->m_DeviceBlitDirection, me->m_orientationRotation.degrees(), bd, dbd, s3eSurfaceGetInt(S3E_SURFACE_WIDTH), s3eSurfaceGetInt(S3E_SURFACE_HEIGHT));
//
//  return 0;
//}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
OGLView* RenderWindowOGLIOS::view() const
{
  return m_view;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderWindowOGLIOS::detectCapabilities()
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
  
  // multitexturing is supported by default
  Device::SetRenderCapability(EGEDevice::RENDER_CAPS_MULTITEXTURE, true);
  
  // VBO is supported by default
  Device::SetRenderCapability(EGEDevice::RENDER_CAPS_VBO, true);

  // combine texture environment mode is supported by default
  Device::SetRenderCapability(EGEDevice::RENDER_CAPS_COMBINE_TEXTURE_ENV, true);
  
  // point sprites are not available by default
  Device::SetRenderCapability(EGEDevice::RENDER_CAPS_POINT_SPRITE, false);
  
  // point sprite size array is not supported by default
  Device::SetRenderCapability(EGEDevice::RENDER_CAPS_POINT_SPRITE_SIZE, false);
  
  // check if FBO is supported
#if GL_OES_framebuffer_object
  glBindFramebuffer         = ::glBindFramebufferOES;
  glDeleteFramebuffers      = ::glDeleteFramebuffersOES;
  glGenFramebuffers         = ::glGenFramebuffersOES;
  glCheckFramebufferStatus  = ::glCheckFramebufferStatusOES;
  glFramebufferTexture2D    = ::glFramebufferTexture2DOES;
  
  Device::SetRenderCapability(EGEDevice::RENDER_CAPS_FBO, true);
#else
  glBindFramebuffer         = ::glBindFramebuffer;
  glDeleteFramebuffers      = ::glDeleteFramebuffers;
  glGenFramebuffers         = ::glGenFramebuffers;
  glCheckFramebufferStatus  = ::glCheckFramebufferStatus;
  glFramebufferTexture2D    = ::glFramebufferTexture2D;

  Device::SetRenderCapability(EGEDevice::RENDER_CAPS_FBO, true);
#endif // GL_OES_framebuffer_object

  // check if map buffer is supported
#if GL_OES_mapbuffer
  glMapBuffer   = ::glMapBufferOES;
  glUnmapBuffer = ::glUnmapBufferOES;
    
  Device::SetRenderCapability(EGEDevice::RENDER_CAPS_MAP_BUFFER, true);
#endif // GL_OES_mapbuffer
  
  // blending functions are supported
#if GL_OES_blend_subtract
  glBlendEquation = ::glBlendEquationOES;
#else
  glBlendEquation = ::glBlendEquation;
#endif // GL_OES_blend_subtract

#if GL_OES_blend_func_separate
  glBlendFuncSeparate = ::glBlendFuncSeparateOES;
#else
  glBlendFuncSeparate = ::glBlendFuncSeparate;
#endif // GL_OES_blend_func_separate
  
  Device::SetRenderCapability(EGEDevice::RENDER_CAPS_BLEND_MINMAX, true);
      
  // check for texture compressions support
#if GL_IMG_texture_compression_pvrtc
  Device::SetRenderCapability(EGEDevice::RENDER_CAPS_TEXTURE_COMPRESSION_PVRTC, true);
#endif // GL_IMG_texture_compression_pvrtc
  
#if !EGE_RENDER_OPENGL_FIXED
  glCreateShaderObject  = ::glCreateShader;
  glShaderSource        = ::glShaderSource;
  glCompileShader       = ::glCompileShader;
  glCreateProgramObject = ::glCreateProgram;
  glAttachObject        = ::glAttachShader;
  glLinkProgram         = ::glLinkProgram;
  glUseProgramObject    = ::glUseProgram;
  glGetUniformLocation  = ::glGetUniformLocation;
  glUniform1f           = ::glUniform1f;
  glUniform2f           = ::glUniform2f;
  glUniform3f           = ::glUniform3f;
  glUniform4f           = ::glUniform4f;
  glUniform1i           = ::glUniform1i;
  glUniform2i           = ::glUniform2i;
  glUniform3i           = ::glUniform3i;
  glUniform4i           = ::glUniform4i;
  glUniformMatrix2fv    = ::glUniformMatrix2fv;
  glUniformMatrix3fv    = ::glUniformMatrix3fv;
  glUniformMatrix4fv    = ::glUniformMatrix4fv;

  Device::SetRenderCapability(EGEDevice::RENDER_CAPS_VERTEX_SHADER, true);
  Device::SetRenderCapability(EGEDevice::RENDER_CAPS_FRAGMENT_SHADER, true);
#endif // !EGE_RENDER_OPENGL_FIXED

  // check 32bit indexing support
#if GL_OES_element_index_uint
  Device::SetRenderCapability(EGEDevice::RENDER_CAPS_ELEMENT_INDEX_UINT, true);
#endif // GL_OES_element_index_uint
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END