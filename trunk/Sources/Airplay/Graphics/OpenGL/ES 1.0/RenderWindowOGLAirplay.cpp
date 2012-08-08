#include "Core/Application/Application.h"
#include "Airplay/Graphics/OpenGL/ES 1.0/RenderWindowOGLAirplay.h"
#include "s3e.h"
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
  create(params);

  // register for surface orientation changes
  if (S3E_RESULT_SUCCESS != s3eSurfaceRegister(S3E_SURFACE_SCREENSIZE, RenderWindowOGLAirplay::OrientationChangeCB, this))
  {
    // error!
    egeWarning() << "Count not register surface orientation change callback.";
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderWindowOGLAirplay::~RenderWindowOGLAirplay()
{
  // unregister surface orientation change callback
  s3eSurfaceUnRegister(S3E_SURFACE_SCREENSIZE, RenderWindowOGLAirplay::OrientationChangeCB);

  destroy();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates Airplay OpenGL window. */
void RenderWindowOGLAirplay::create(const Dictionary& params)
{
  bool error = false;

  // decompose param list
  Dictionary::const_iterator iterColorBits = params.find(EGE_RENDER_TARGET_PARAM_COLOR_BITS);
  Dictionary::const_iterator iterDepthBits = params.find(EGE_RENDER_WINDOW_PARAM_DEPTH_BITS);
  Dictionary::const_iterator iterLandscape = params.find(EGE_ENGINE_PARAM_LANDSCAPE_MODE);

  // check if required parameters are NOT present
  if (iterColorBits == params.end() || iterDepthBits == params.end())
  {
    // error!
    return;
  }

  bool landscape = (iterLandscape != params.end()) ? iterLandscape->second.toBool(&error) : false;

  // set default paramerter values
  s32 colorBits = iterColorBits->second.toInt(&error);
  s32 depthBits = iterDepthBits->second.toInt(&error);

  if (error)
  {
    // error!
    return;
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
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Destorys Airplay OpenGL window. */
void RenderWindowOGLAirplay::destroy()
{
  releaseCurrentContext();
  eglDestroyContext(m_eglDisplay, m_eglContext);
  eglDestroySurface(m_eglDisplay, m_eglSurface);
  eglTerminate(m_eglDisplay);

  m_eglDisplay = EGL_NO_DISPLAY;
  m_eglContext = EGL_NO_CONTEXT;
  m_eglSurface = EGL_NO_SURFACE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! RenderWindow override. Makes rendering context calling thread's current rendering context. */
EGEResult RenderWindowOGLAirplay::makeCurrentContext()
{
  return (EGL_FALSE == eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext)) ? EGE_ERROR : EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! RenderWindow override. Removes calling thread's current rendering context. */
void RenderWindowOGLAirplay::releaseCurrentContext()
{
  eglMakeCurrent(m_eglDisplay, NULL, NULL, NULL);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! RenderWindow override. Shows frame buffer. */
void RenderWindowOGLAirplay::showFrameBuffer()
{
  eglSwapBuffers(m_eglDisplay, m_eglSurface);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Returns TRUE if object is valid. */
bool RenderWindowOGLAirplay::isValid() const
{
  if (!RenderWindow::isValid())
  {
    // not valid
    return false;
  }

  return (EGL_NO_SURFACE != m_eglSurface) && (EGL_NO_CONTEXT != m_eglContext) && (EGL_NO_DISPLAY != m_eglDisplay);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! RenderWindow override. Enables/Disables fullscreen mode. */
EGEResult RenderWindowOGLAirplay::enableFullScreen(s32 width, s32 height, bool enable)
{
  EGE_UNUSED(width);
  EGE_UNUSED(height);
  EGE_UNUSED(enable);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! RenderTarget override. Returns TRUE if texture flipping is required for this render target. */
bool RenderWindowOGLAirplay::requiresTextureFlipping() const
{
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Orientation change callback. */
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

EGE_NAMESPACE_END