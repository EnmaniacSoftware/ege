#include "iOS/Graphics/OpenGL/RenderWindowOGLIOS.h"
#include "Core/ComplexTypes.h"
#include "EGEEngine.h"
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
RenderWindowOGLIOS::RenderWindowOGLIOS(Engine& engine, const Dictionary& params)
: RenderWindow(params)
, m_engine(engine)
, m_view(nil)
, m_viewController(nil)
, m_window(nil)
, m_EAGLContext(nil)
, m_colorBuffer(0)
, m_depthBuffer(0)
, m_frameBuffer(0)
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
  
  // get screen size unaffected by scale factor
  // NOTE: scale factor will be adjusted during UIView initialization
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
  m_viewController = [[ViewController alloc] initWithApplication: &engine()];
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
    
  // generate main frame buffer
  glGenFramebuffers(1, &m_frameBuffer);
  OGL_CHECK();
  glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
  OGL_CHECK();
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colorBuffer);
  OGL_CHECK();
  
  // generate window depth buffer if requested
  if (0 != depthBits)
  {
    glGenRenderbuffers(1, &m_depthBuffer);
    OGL_CHECK();
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
    OGL_CHECK();
    
    //		if( multiSampling_ )
    //			glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER_OES, samplesToUse_, depthFormat_,backingWidth_, backingHeight_);
    //		else
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_physicalWidth, m_physicalHeight);
    OGL_CHECK();
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);
		OGL_CHECK();
  }
  
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
  if ( ! engine().eventManager()->registerListener(this))
  {
    // error!
    return EGE_ERROR;
  }
  
  // set initial orientation
  UIInterfaceOrientation nativeOrientation = [[UIApplication sharedApplication] statusBarOrientation];
  int orientation = [m_viewController convertUIOrientation: nativeOrientation];
  setOrientation(static_cast<DeviceOrientation>(orientation));
  
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderWindowOGLIOS::destroy()
{
  // unregister from events
  engine().eventManager()->unregisterListener(this);
  
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
  // present render buffer
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
bool RenderWindowOGLIOS::isAutoRotated() const override
{
  return true;
}
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
  
  for (StringArray::ConstIterator it = extensionArray.begin(); it != extensionArray.end(); ++it)
  {
    egeDebug(KOpenGLDebugName) << "Available OGL extension:" << *it;
  }
  
	GLint value;
  
  // get number of texture units
	glGetIntegerv(GL_MAX_TEXTURE_UNITS, &value);
  Device::SetTextureUnitsCount(static_cast<u32>(value));
  
  // detect maximal texture size
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value);
  Device::SetTextureMaxSize(static_cast<u32>(value));
  
  // multitexturing is supported by default
  Device::SetRenderCapability(ERenderCapabilityMultitexturing, true);
  
  // VBO is supported by default
  // TAGE - disable for time being until VBO implementation is more efficient
  Device::SetRenderCapability(ERenderCapabilityVertexBufferObjects, false);

  // point sprites are not available by default
  Device::SetRenderCapability(ERenderCapabilityPointSprites, false);
  
  // check if FBO is supported
#if GL_OES_framebuffer_object
  glBindFramebuffer         = ::glBindFramebufferOES;
  glDeleteFramebuffers      = ::glDeleteFramebuffersOES;
  glGenFramebuffers         = ::glGenFramebuffersOES;
  glCheckFramebufferStatus  = ::glCheckFramebufferStatusOES;
  glFramebufferTexture2D    = ::glFramebufferTexture2DOES;
  glRenderbufferStorage     = ::glRenderbufferStorageOES;
  
  Device::SetRenderCapability(ERenderCapabilityFrameBufferObjects, true);
#else
  glBindFramebuffer         = ::glBindFramebuffer;
  glDeleteFramebuffers      = ::glDeleteFramebuffers;
  glGenFramebuffers         = ::glGenFramebuffers;
  glCheckFramebufferStatus  = ::glCheckFramebufferStatus;
  glFramebufferTexture2D    = ::glFramebufferTexture2D;
  glRenderbufferStorage     = ::glRenderbufferStorage;

  Device::SetRenderCapability(ERenderCapabilityFrameBufferObjects, true);
#endif // GL_OES_framebuffer_object

  // check if map buffer is supported
#if GL_OES_mapbuffer
  glMapBuffer   = ::glMapBufferOES;
  glUnmapBuffer = ::glUnmapBufferOES;
    
  Device::SetRenderCapability(ERenderCapabilityMapBuffer, true);
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
        
  // check for texture compressions support
#if GL_IMG_texture_compression_pvrtc
  Device::SetRenderCapability(ERenderCapabilityTextureCompressionPVRTC, true);
#endif // GL_IMG_texture_compression_pvrtc
  
#if !EGE_RENDERING_OPENGL_FIXED
  glCreateShader              = ::glCreateShader;
  glShaderSource              = ::glShaderSource;
  glCompileShader             = ::glCompileShader;
  glCreateProgram             = ::glCreateProgram;
  glAttachShader              = ::glAttachShader;
  glLinkProgram               = ::glLinkProgram;
  glUseProgram                = ::glUseProgram;
  glGetUniformLocation        = ::glGetUniformLocation;
  glUniform1f                 = ::glUniform1f;
  glUniform2f                 = ::glUniform2f;
  glUniform3f                 = ::glUniform3f;
  glUniform4f                 = ::glUniform4f;
  glUniform1i                 = ::glUniform1i;
  glUniform2i                 = ::glUniform2i;
  glUniform3i                 = ::glUniform3i;
  glUniform4i                 = ::glUniform4i;
  glUniformMatrix2fv          = ::glUniformMatrix2fv;
  glUniformMatrix3fv          = ::glUniformMatrix3fv;
  glUniformMatrix4fv          = ::glUniformMatrix4fv;
  glDisableVertexAttribArray  = ::glDisableVertexAttribArray;
  glEnableVertexAttribArray   = ::glEnableVertexAttribArray;
  glGetAttribLocation         = ::glGetAttribLocation;
  glVertexAttribPointer       = ::glVertexAttribPointer;
  glGetActiveAttrib           = ::glGetActiveAttrib;

  Device::SetRenderCapability(EGEDevice::RENDER_CAPS_VERTEX_SHADER, true);
  Device::SetRenderCapability(EGEDevice::RENDER_CAPS_FRAGMENT_SHADER, true);
#endif // !EGE_RENDERING_OPENGL_FIXED

#if GL_OES_vertex_array_object
  glGenVertexArrays     = ::glGenVertexArraysOES;
  glIsVertexArray       = ::glIsVertexArrayOES;
  glDeleteVertexArrays  = ::glDeleteVertexArraysOES;
  glBindVertexArray     = ::glBindVertexArrayOES;
  
  Device::SetRenderCapability(ERenderCapabilityVertexArrayObjects, true);
#endif // GL_OES_vertex_array_object
  
  // check 32bit indexing support
#if GL_OES_element_index_uint
  Device::SetRenderCapability(ERenderCapabilityElementIndexUnsignedInt, true);
#endif // GL_OES_element_index_uint

  // auto mipmapping support
  glGenerateMipmap = ::glGenerateMipmap;
  Device::SetRenderCapability(ERenderCapabilityAutoMipmapping, true);

  // at least one check at the end
  OGL_CHECK()
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderWindowOGLIOS::onEventRecieved(PEvent event)
{
  // process
  switch (event->id())
  {
    case EGE_EVENT_ID_INTERNAL_ORIENTATION_CHANGED:
      
      setOrientation(static_cast<DeviceOrientation>(ege_cast<Integer*>(event->data())->value()));
      break;
      
    default:
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderWindowOGLIOS::setOrientation(DeviceOrientation orientation)
{
  // resize render buffers
  resizeRenderBuffers();
  
  // check if NOT auto-rotated
  if ( ! isAutoRotated())
  {
    // process accroding to blit direction
    switch (orientation)
    {
      case EOrientationPortrait:
    
        m_orientationRotation.fromDegrees(0.0f);
        m_width   = m_physicalWidth;
        m_height  = m_physicalHeight;
        break;
    
      case EOrientationPortraitUpsideDown:
    
        m_orientationRotation.fromDegrees(180.0f);
        m_width   = m_physicalWidth;
        m_height  = m_physicalHeight;
        break;

      case EOrientationLandscapeLeft:
    
        m_orientationRotation.fromDegrees(90.0f);
        m_width   = m_physicalHeight;
        m_height  = m_physicalWidth;
        break;
    
      case EOrientationLandscapeRight:
    
        m_orientationRotation.fromDegrees(270.0f);
        m_width   = m_physicalHeight;
        m_height  = m_physicalWidth;
        break;
    
      case EOrientationFaceUp:
      case EOrientationFaceDown:
    
        m_orientationRotation.fromDegrees(0.0f);
        m_width   = m_physicalWidth;
        m_height  = m_physicalHeight;
        break;
      
      default:

        egeWarning(KOpenGLDebugName) << "Unknown orientation!";
        break;
    }
  }
  else
  {
    // with auto-rotation physical size is the same as logical one
    m_orientationRotation.fromDegrees(0.0f);
    m_width   = m_physicalWidth;
    m_height  = m_physicalHeight;
  }
  
  // propagate to rest of the framework
  EGE_ASSERT(engine().eventManager());
  engine().eventManager()->send(EGE_EVENT_ID_CORE_ORIENTATION_CHANGED);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderWindowOGLIOS::resizeRenderBuffers()
{
	// this effectively updates render buffer size
  glBindRenderbuffer(GL_RENDERBUFFER, m_colorBuffer);
  OGL_CHECK();
  
  // rebind buffers to EAGL layer
  [m_EAGLContext renderbufferStorage: GL_RENDERBUFFER fromDrawable: m_view.mEAGLLayer];

  // retrieve render buffer dimensions
  glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &m_physicalWidth);
  glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &m_physicalHeight);
	
  // generate window depth buffer if requested
  if (0 != m_depthBuffer)
  {
    // TAGE - should we delete old one ?
    
    glGenRenderbuffers(1, &m_depthBuffer);
    OGL_CHECK();
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
    OGL_CHECK();
    
    //		if( multiSampling_ )
    //			glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER_OES, samplesToUse_, depthFormat_,backingWidth_, backingHeight_);
    //		else
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_physicalWidth, m_physicalHeight);
    OGL_CHECK();
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);
		OGL_CHECK();
  }

  // check frame buffer integrity
  if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
	{
    egeCritical(KOpenGLDebugName) << "Could not resize render buffers!";
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UIWindow* RenderWindowOGLIOS::window() const
{
  return m_window;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Engine& RenderWindowOGLIOS::engine() const
{
  return m_engine;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END