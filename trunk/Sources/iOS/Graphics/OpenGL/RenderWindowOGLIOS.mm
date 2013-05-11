#include "iOS/Graphics/OpenGL/RenderWindowOGLIOS.h"
#include "Core/ComplexTypes.h"
#include "EGEApplication.h"
#include "EGEMath.h"
#include "EGEDevice.h"
#include "EGEDebug.h"
#import <UIKit/UIWindow.h>
#import <UIKit/UIScreen.h>
#import "iOS/Graphics/OpenGL/ViewOGL.h"
#import "iOS/Application/ViewController.h"

#if EGE_RENDER_OPENGL_FIXED
  #import <OpenGLES/ES1/glext.h>
#endif // EGE_RENDER_OPENGL_FIXED

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderWindowOGLIOS::RenderWindowOGLIOS(Application* app, const Dictionary& params) : RenderWindow(app, params),
                                                                                     m_view(nil),
                                                                                     m_viewController(nil),
                                                                                     m_window(nil),
                                                                                     m_EAGLContext(nil),
                                                                                     m_colorBuffer(0),
                                                                                     m_depthBuffer(0),
                                                                                     m_frameBuffer(0)
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
  bool error = false;

  // decompose param list
  Dictionary::const_iterator iterColorBits = params.find(EGE_RENDER_TARGET_PARAM_COLOR_BITS);
  Dictionary::const_iterator iterDepthBits = params.find(EGE_RENDER_WINDOW_PARAM_DEPTH_BITS);
  
  // set default paramerter values
  s32 colorBits = iterColorBits->second.toInt(&error);
  s32 depthBits = iterDepthBits->second.toInt(&error);
  
  if (error)
  {
    // error!
    return EGE_ERROR;
  }
  
  // get screen size
  CGRect screenBounds = [[UIScreen mainScreen] bounds];
  
  // create full screen view
  NSString* pixelFormat = (16 >= colorBits) ? kEAGLColorFormatRGB565 : kEAGLColorFormatRGBA8;
  m_view = [[OGLView alloc] initWithFrame: screenBounds andPixelFormat: pixelFormat];
  if (nil == m_view)
  {
    // error!r
    return EGE_ERROR_NO_MEMORY;
  }

  // create controller view
  m_viewController = [[ViewController alloc] initWithNibName: nil bundle: nil];
  if (nil == m_view)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }
  
  // make sure to grab entire screen
  m_viewController.wantsFullScreenLayout = YES;
  
	// set view controller's view
	[m_viewController setView: m_view];
	 
  // create window
  m_window = [[UIWindow alloc] initWithFrame: screenBounds];
  if (nil == m_window)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }
  
  // attach view to window
  m_window.backgroundColor = [UIColor whiteColor];
  m_window.rootViewController = m_viewController;
  
  // create rendering context
  EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES1;
  m_EAGLContext = [[EAGLContext alloc] initWithAPI: api];
  if (nil == m_EAGLContext)
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

  // generate window color buffer
  glGenRenderbuffers(1, &m_colorBuffer);
  OGL_CHECK();
  glBindRenderbuffer(GL_RENDERBUFFER, m_colorBuffer);
  OGL_CHECK();

  // bind it to drawing layer
  [m_EAGLContext renderbufferStorage: GL_RENDERBUFFER fromDrawable: m_view.mEAGLLayer];
  
  // retrieve render buffer dimensions
  glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &m_physicalWidth);
  glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &m_physicalHeight);
  
  // get current orientation screen dimensions
  m_width  = m_physicalWidth;
  m_height = m_physicalHeight;
  
  // generate window depth buffer if requested
  if (0 != depthBits)
  {
    glGenRenderbuffers(1, &m_depthBuffer);
    OGL_CHECK();
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
    OGL_CHECK();

    // convert to native depth format
    GLenum depthFormat = (0 != depthBits) ? GL_DEPTH_COMPONENT16 : 0;
    
    //		if( multiSampling_ )
//			glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER_OES, samplesToUse_, depthFormat_,backingWidth_, backingHeight_);
//		else
    glRenderbufferStorage(GL_RENDERBUFFER, depthFormat, m_physicalWidth, m_physicalHeight);
    OGL_CHECK();
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);
		OGL_CHECK();
  }
    
  // generate main frame buffer
  glGenFramebuffers(1, &m_frameBuffer);
  OGL_CHECK();
  glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
  OGL_CHECK();
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colorBuffer);
  OGL_CHECK();
  
  // make it visible
  [m_window makeKeyAndVisible];
  
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

  // register for events
  if ( ! app()->eventManager()->addListener(this))
  {
    // error!
    return EGE_ERROR;
  }
  
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderWindowOGLIOS::destroy()
{
  // unregister from events
  app()->eventManager()->removeListener(this);
  
  if (0 != m_frameBuffer)
  {
    glDeleteFramebuffers(1, &m_frameBuffer);
    m_frameBuffer = 0;
  }

  if (0 != m_colorBuffer)
  {
    glDeleteBuffers(1, &m_colorBuffer);
    m_colorBuffer = 0;
  }

  if (0 != m_depthBuffer)
  {
    glDeleteBuffers(1, &m_depthBuffer);
    m_depthBuffer = 0;
  }
  
  [EAGLContext setCurrentContext: nil];
  [m_EAGLContext release];
  m_EAGLContext = nil;
  
  [m_viewController release];
  [m_view release];
  [m_window release];
  
  m_view            = nil;
  m_viewController  = nil;
  m_window          = nil;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderWindowOGLIOS::showFrameBuffer()
{
  [m_EAGLContext presentRenderbuffer: GL_RENDERBUFFER];
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
  glRenderbufferStorage     = ::glRenderbufferStorageOES;
  
  Device::SetRenderCapability(EGEDevice::RENDER_CAPS_FBO, true);
#else
  glBindFramebuffer         = ::glBindFramebuffer;
  glDeleteFramebuffers      = ::glDeleteFramebuffers;
  glGenFramebuffers         = ::glGenFramebuffers;
  glCheckFramebufferStatus  = ::glCheckFramebufferStatus;
  glFramebufferTexture2D    = ::glFramebufferTexture2D;
  glRenderbufferStorage     = ::glRenderbufferStorage;

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
void RenderWindowOGLIOS::onEventRecieved(PEvent event)
{
  // process
  switch (event->id())
  {
    case EGE_EVENT_ID_INTERNAL_ORIENTATION_CHANGED:
      
      setOrientation(static_cast<EGEDevice::Orientation>(ege_cast<Integer*>(event->data())->value()));
      break;
      
    default:
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderWindowOGLIOS::setOrientation(EGEDevice::Orientation orientation)
{
  // process accroding to blit direction
  switch (orientation)
  {
    case EGEDevice::EOrientationPortrait:
    
      m_orientationRotation.fromDegrees(0.0f);
      m_width   = m_physicalWidth;
      m_height  = m_physicalHeight;
      break;
    
    case EGEDevice::EOrientationPortraitUpsideDown:
    
      m_orientationRotation.fromDegrees(180.0f);
      m_width   = m_physicalWidth;
      m_height  = m_physicalHeight;
      break;

    case EGEDevice::EOrientationLandscapeLeft:
    
      m_orientationRotation.fromDegrees(90.0f);
      m_width   = m_physicalHeight;
      m_height  = m_physicalWidth;
      break;
    
    case EGEDevice::EOrientationLandscapeRight:
    
      m_orientationRotation.fromDegrees(270.0f);
      m_width   = m_physicalHeight;
      m_height  = m_physicalWidth;
      break;
    
    case EGEDevice::EOrientationFaceUp:
    case EGEDevice::EOrientationFaceDown:
    
      m_orientationRotation.fromDegrees(0.0f);
      m_width   = m_physicalWidth;
      m_height  = m_physicalHeight;
      break;
      
    default:

      egeWarning() << "Unknown orientation!";
      break;
  }
  
  // propagate to rest of the framework
  EGE_ASSERT(app() && app()->eventManager());
  app()->eventManager()->send(EGE_EVENT_ID_CORE_ORIENTATION_CHANGED);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END