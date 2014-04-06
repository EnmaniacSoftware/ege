#include "Core/Application/Application.h"
#include "Win32/Graphics/OpenGL/RenderWindowOGLWin32.h"
#include "EGEOpenGL.h"
#include "EGEEvent.h"
#include "EGEInput.h"
#include "EGEMath.h"
#include "EGEDevice.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderWindowOGLWin32::RenderWindowOGLWin32(Application* app, const Dictionary& params) : RenderWindow(app, params), 
                                                                                         m_hWnd(NULL), 
                                                                                         m_hDC(NULL), 
                                                                                         m_hRC(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderWindowOGLWin32::~RenderWindowOGLWin32()
{
  destroy();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult RenderWindowOGLWin32::construct(const Dictionary& params)
{
  WNDCLASS sWndclass = { 0 };

  DWORD dwStyles;

  bool error = false;

  // decompose param list
  Dictionary::const_iterator iterFullscreen  = params.find(EGE_RENDER_WINDOW_PARAM_FULLSCREEN);
  Dictionary::const_iterator iterLandscape   = params.find(EGE_ENGINE_PARAM_LANDSCAPE_MODE);

  bool landscape  = (iterLandscape != params.end()) ? iterLandscape->second.toBool(&error) : false;
  bool fullscreen = (iterFullscreen != params.end()) ? iterFullscreen->second.toBool(&error) : false;

  if (error)
  {
    // error!
    return EGE_ERROR;
  }

  // apply dimensions according to landscape requirement
  s32 width  = m_width;
  s32 height = m_height;

  if (landscape)
  {
    m_height = Math::Min(width, height);
    m_width  = Math::Max(width, height);
  }
  else
  {
    m_height = Math::Max(width, height);
    m_width  = Math::Min(width, height);
  }

  // there are no fixed restriction for window render target in Windows
  m_physicalWidth  = m_width;
  m_physicalHeight = m_height;

  // apply zoom to physical size
  m_physicalWidth  = static_cast<s32>(m_physicalWidth * zoom());
  m_physicalHeight = static_cast<s32>(m_physicalHeight * zoom());

  // setup wndclass
	sWndclass.style         = CS_HREDRAW | CS_VREDRAW;	
  sWndclass.cbWndExtra    = 4;
  sWndclass.lpfnWndProc   = RenderWindowOGLWin32::WinProc;						
	sWndclass.hInstance     = GetModuleHandle(NULL);						
	sWndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	sWndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);		
	sWndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	sWndclass.lpszClassName = TEXT("RenderWindowOGLWin32ClassName");			

  // register window class
	if (!RegisterClass(&sWndclass))
  {
    // error!
    return EGE_ERROR;
  }

  // enable fullscreen window
	if (fullscreen)
	{				
    // setup window styles for fullscreen mode
		dwStyles = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    
    // enable fullscreen
    if (EGE_SUCCESS != enableFullScreen(m_width, m_height, true))
    {
      // error!
      return EGE_ERROR;
    }
	}
  else
  {
    // setup window styles for windowed mode
	  dwStyles = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
  }
	
  // resize window up to state client area matches requested dimensions
	RECT rcWindow;

	rcWindow.left	  = 0;
	rcWindow.right	= m_physicalWidth;
	rcWindow.top	  = 0;		
	rcWindow.bottom = m_physicalHeight;

	AdjustWindowRect(&rcWindow, dwStyles, false);

  // create the window
  if (NULL == (m_hWnd = CreateWindowA("RenderWindowOGLWin32ClassName", title().c_str(), dwStyles, 0, 0, rcWindow.right - rcWindow.left, 
                                      rcWindow.bottom - rcWindow.top, NULL, NULL, GetModuleHandle(NULL), NULL)))
  {
    // error!
    return EGE_ERROR;
  }

  // get window's DC
  if (NULL == (m_hDC = GetDC(m_hWnd)))
  {
    // error!
    destroy();
    return EGE_ERROR;
  }

  // setup pixel format
  if (EGE_SUCCESS != setupPixelFormat(params))
  {
    // error!
    destroy();
    return EGE_ERROR;
  }
  
  // create rendering context
  HGLRC hRC;
  if (NULL == (hRC = wglCreateContext(m_hDC)))
  {
    // error!
    destroy();
    return EGE_ERROR;
  }

  // assign rendering context to current thread
  if (FALSE == wglMakeCurrent(m_hDC, hRC))
  {
    // error!
    destroy();
    return EGE_ERROR;
  }

#if EGE_RENDERING_OPENGL_FIXED
  // requesting 1.5 context via extension
  int attributes[] = 
  {  
    WGL_CONTEXT_MAJOR_VERSION_ARB, 1,
    WGL_CONTEXT_MINOR_VERSION_ARB, 5,
    0  
  };  
#else
  // requesting 3.x+ context via extension
  int attributes[] = 
  {  
    WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
    WGL_CONTEXT_MINOR_VERSION_ARB, 0,
    //WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
    0  
  };  
#endif // EGE_RENDERING_OPENGL_FIXED

  // retrieve required wiggle function pointer
  wglCreateContextAttribs = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));
  if (NULL == wglCreateContextAttribs)
  {
    // error!
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    destroy();
    return EGE_ERROR;
  }

  m_hRC = wglCreateContextAttribs(m_hDC, 0, attributes);

  // delete old rendering context
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(hRC);

  // check if new rendering context failed to create
  if (NULL == m_hRC)
  {
    // error!
    destroy();
    return EGE_ERROR;
  }

  // assign rendering context to current thread
  if (FALSE == wglMakeCurrent(m_hDC, m_hRC))
  {
    // error!
    destroy();
    return EGE_ERROR;
  }

  // detect capabilities
  detectCapabilities();

  // set pointer to this control in extra wnd data
  SetWindowLongPtr(m_hWnd, 0, reinterpret_cast<LONG>(this));

  //// store render target dimensions
  //setDimensions( uiWindowWidth, uiWindowHeight );

  // show window and update it
	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);		

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderWindowOGLWin32::destroy()
{
  // check if main window is created
  if (NULL != m_hWnd)
  {
    // check if there is any DC
    if (NULL != m_hDC)
    {
      // check if there is any RC 
      if (NULL != m_hRC)
      {
        // remove rendering context from current thread
        wglMakeCurrent(NULL, NULL);

        // disconnect it from window
        wglDeleteContext(m_hRC);
        m_hRC = NULL;
      }
      
      // delete DC
      ReleaseDC(m_hWnd, m_hDC);
      m_hDC = NULL;
    }

    // clean up
    SetWindowLongPtr(m_hWnd, 0, 0);

    // destroy main window
    DestroyWindow(m_hWnd);
    m_hWnd = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK RenderWindowOGLWin32::WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  RenderWindowOGLWin32* me = reinterpret_cast<RenderWindowOGLWin32*>(GetWindowLongPtr(hWnd, 0));

  EventManager* eventManager = NULL;
  if (NULL != me)
  {
    eventManager = me->app()->eventManager();
  }

  // detrmine current keyboard modifiers
  KeyboardModifiers keyboardModifiers = KM_NONE;
  if (GetAsyncKeyState(VK_MENU) & 0x8000)
  {
    keyboardModifiers |= KM_ALT;
  }

  if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
  {
    keyboardModifiers |= KM_CTRL;
  }

  if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
  {
    keyboardModifiers |= KM_SHIFT;
  }

  s32 x;
  s32 y;

  // process according to message
  switch (msg)
  {
    case WM_LBUTTONDOWN:

      if (NULL != eventManager)
      {
        x = static_cast<s32>(LOWORD(lParam) / me->zoom());
        y = static_cast<s32>(HIWORD(lParam) / me->zoom());

        PObject data = ege_new PointerData(ACTION_BUTTON_DOWN, BUTTON_LEFT, keyboardModifiers, x, y, 0);
        eventManager->send(EGE_EVENT_ID_INTERNAL_POINTER_DATA, data);
      }
      return 0;

    case WM_LBUTTONUP:

      if (NULL != eventManager)
      {
        x = static_cast<s32>(LOWORD(lParam) / me->zoom());
        y = static_cast<s32>(HIWORD(lParam) / me->zoom());

        PObject data = ege_new PointerData(ACTION_BUTTON_UP, BUTTON_LEFT, keyboardModifiers, x, y, 0);
        eventManager->send(EGE_EVENT_ID_INTERNAL_POINTER_DATA, ege_pcast<PObject>(data));
      }
      return 0;

    case WM_RBUTTONDOWN:

      if (NULL != eventManager)
      {
        x = static_cast<s32>(LOWORD(lParam) / me->zoom());
        y = static_cast<s32>(HIWORD(lParam) / me->zoom());

        PObject data = ege_new PointerData(ACTION_BUTTON_DOWN, BUTTON_RIGHT, keyboardModifiers, x, y, 0);
        eventManager->send(EGE_EVENT_ID_INTERNAL_POINTER_DATA, data);
      }
      return 0;

    case WM_RBUTTONUP:

      if (NULL != eventManager)
      {
        x = static_cast<s32>(LOWORD(lParam) / me->zoom());
        y = static_cast<s32>(HIWORD(lParam) / me->zoom());

        PObject data = ege_new PointerData(ACTION_BUTTON_UP, BUTTON_RIGHT, keyboardModifiers, x, y, 0);
        eventManager->send(EGE_EVENT_ID_INTERNAL_POINTER_DATA, data);
      }
      return 0;

    case WM_MOUSEMOVE:

      if (NULL != eventManager)
      {
        x = static_cast<s32>(LOWORD(lParam) / me->zoom());
        y = static_cast<s32>(HIWORD(lParam) / me->zoom());

        PObject data = ege_new PointerData(ACTION_MOVE, BUTTON_NONE, keyboardModifiers, x, y, 0);
        eventManager->send(EGE_EVENT_ID_INTERNAL_POINTER_DATA, data);
      }
      return 0;

    case WM_SIZE:

 //     ::engine->resizeWindow( LOWORD( lParam ), HIWORD( lParam ) );
      return 0;

    case WM_MOUSEWHEEL:

      // check if we are running Win32 mouse
      //if ( ::pcMouse->getType() == MOUSETYPE_WIN32 )
      //{
      //  ( ( CEnmMouse_Win32* ) ::pcMouse )->setDisplacementZ( HIWORD( wParam ) );
      //}
      return 0;

    case WM_CLOSE:
    case WM_QUIT:

      // shut down engine
      if (NULL != me)
      {
        me->app()->quit();
      }
      return 0;

    case WM_SETFOCUS:

      if (NULL != eventManager)
      {
        eventManager->send(EGE_EVENT_ID_CORE_APP_RESUME);
      }
      return 0;

    case WM_KILLFOCUS:

      if (NULL != eventManager)
      {
        eventManager->send(EGE_EVENT_ID_CORE_APP_PAUSE);
      }
      return 0;
  } 

  return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult RenderWindowOGLWin32::setupPixelFormat(const Dictionary& params)
{
  PIXELFORMATDESCRIPTOR sPFD; 

  s32 pixelFormat;

  bool error = false;

  // decompose param list
  Dictionary::const_iterator iterColorBits = params.find(EGE_RENDER_TARGET_PARAM_COLOR_BITS);
  Dictionary::const_iterator iterDepthBits = params.find(EGE_RENDER_WINDOW_PARAM_DEPTH_BITS);

  // check if required parameters are NOT present
  if (iterColorBits == params.end())
  {
    // error!
    return EGE_ERROR_BAD_PARAM;
  }

  s32 colorBits   = iterColorBits->second.toInt(&error);
  s32 depthBits   = (iterDepthBits != params.end()) ? iterDepthBits->second.toInt(&error) : 0;
  s32 accumBits   = 0;
  s32 stencilBits = 0;

  if (error)
  {
    // error!
    return EGE_ERROR_BAD_PARAM;
  }

  sPFD.nSize        = sizeof (PIXELFORMATDESCRIPTOR);
  sPFD.nVersion     = 1;									
  sPFD.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
  sPFD.dwLayerMask  = PFD_MAIN_PLANE;
  sPFD.iPixelType   = PFD_TYPE_RGBA;
  sPFD.cColorBits   = static_cast<BYTE>(colorBits);
  sPFD.cDepthBits   = static_cast<BYTE>(depthBits);
  sPFD.cAccumBits   = static_cast<BYTE>(accumBits);			
  sPFD.cStencilBits = static_cast<BYTE>(stencilBits);

  // this gets us a pixel format that best matches the one passed in from the device
  if (0 == (pixelFormat = ChoosePixelFormat(m_hDC, &sPFD))) 
  {
    // error!
    return EGE_ERROR; 
  }

  //// check if FSAA NOT requested or NOT supported
  //if ( uiFSAA == 0 || glExt::WGL_ARB_multisample == false || 
  //     glExt::miMultisamplePixelFormats.find( uiFSAA ) == glExt::miMultisamplePixelFormats.end() )
  //{
  //  // this gets us a pixel format that best matches the one passed in from the device
  //  if ( ( iPixelFormat = ::ChoosePixelFormat( m_hDC, &pfd ) ) == false ) 
  //  {
  //    // error!
  //    return false; 
  //  }
  //}
  //else
  //{
  //  // get pixel format for requested FSAA
  //  iPixelFormat = glExt::miMultisamplePixelFormats.find( uiFSAA )->second;
  //}

  // this sets the pixel format that we extracted from above
  if (FALSE == SetPixelFormat(m_hDC, pixelFormat, &sPFD)) 
  {
    // error!
    return EGE_ERROR; 
  }

  // obtain a detailed description of that pixel format  
  if (0 != DescribePixelFormat(m_hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &sPFD))
  {
    // supply surface data for for device info
    Device::SetSurfaceRedChannelBitsCount(static_cast<u32>(sPFD.cRedBits));
    Device::SetSurfaceGreenChannelBitsCount(static_cast<u32>(sPFD.cGreenBits));
    Device::SetSurfaceBlueChannelBitsCount(static_cast<u32>(sPFD.cBlueBits));
    Device::SetSurfaceAlphaChannelBitsCount(static_cast<u32>(sPFD.cAlphaBits));
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult RenderWindowOGLWin32::enableFullScreen(s32 width, s32 height, bool enable)
{
	DEVMODE sDMSettings;

	memset(&sDMSettings, 0, sizeof(DEVMODE));

  if (enable)
  {
    // get current settings -- This function fills our the settings
    // this makes sure NT and Win98 machines change correctly
    if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &sDMSettings))
    {
      // error!
	    return EGE_ERROR;
    }

    sDMSettings.dmPelsWidth	 = width;
    sDMSettings.dmPelsHeight = height;

    // check if we didn't recieved a good return message from the function
    if (DISP_CHANGE_SUCCESSFUL != ChangeDisplaySettings(&sDMSettings, CDS_FULLSCREEN))
    {
      // error!
      return EGE_ERROR;
    }
  }
  else
  {
		ChangeDisplaySettings(NULL, 0);
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderWindowOGLWin32::showFrameBuffer()
{
  SwapBuffers(m_hDC);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool RenderWindowOGLWin32::isValid() const
{
  if (!RenderWindow::isValid())
  {
    // not valid
    return false;
  }

  return (NULL != m_hWnd) && (NULL != m_hDC) && (NULL != m_hRC);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool RenderWindowOGLWin32::requiresTextureFlipping() const
{
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderWindowOGLWin32::detectCapabilities()
{
  StringArray extensionArray;

  // get list of all extensions
#if EGE_RENDERING_OPENGL_FIXED
  String extensionString(reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS)));
  if ( ! extensionString.empty())
  {
    extensionArray = extensionString.split(" ");
  }
#endif // EGE_RENDERING_OPENGL_FIXED
  
  if (extensionArray.empty())
  {
    // determine if glGetStringi is available
    glGetStringi = reinterpret_cast<PFNGLGETSTRINGIPROC>(wglGetProcAddress("glGetStringi"));
    if (NULL != glGetStringi)
    {
      // get number of extensions
      GLint extensionsCount;
      glGetIntegerv(GL_NUM_EXTENSIONS, &extensionsCount);

      // process extensions
      for (GLint i = 0; i < extensionsCount; ++i)
      {
        extensionArray.push_back(reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i)));
      }
    }
  }

  for (StringArray::const_iterator it = extensionArray.begin(); it != extensionArray.end(); ++it)
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

  // check if multitexturing is supported
  if (extensionArray.contains("GL_ARB_multitexture"))
  {
    glClientActiveTexture = reinterpret_cast<PFNGLCLIENTACTIVETEXTUREARBPROC>(wglGetProcAddress("glClientActiveTextureARB"));
    glActiveTexture       = reinterpret_cast<PFNGLACTIVETEXTUREARBPROC>(wglGetProcAddress("glActiveTextureARB"));

    if ((NULL != glClientActiveTexture) && (NULL != glActiveTexture))
    {
      Device::SetRenderCapability(EGEDevice::RENDER_CAPS_MULTITEXTURE, true);
    }
  }

  // check if frame buffer object is supported
  if (extensionArray.contains("GL_EXT_framebuffer_object"))
  {
    glBindFramebuffer         = reinterpret_cast<PFNGLBINDFRAMEBUFFERPROC>(wglGetProcAddress("glBindFramebufferEXT"));
    glDeleteFramebuffers      = reinterpret_cast<PFNGLDELETEFRAMEBUFFERSPROC>(wglGetProcAddress("glDeleteFramebuffersEXT"));
    glGenFramebuffers         = reinterpret_cast<PFNGLGENFRAMEBUFFERSPROC>(wglGetProcAddress("glGenFramebuffersEXT"));
    glCheckFramebufferStatus  = reinterpret_cast<PFNGLCHECKFRAMEBUFFERSTATUSPROC>(wglGetProcAddress("glCheckFramebufferStatusEXT"));
    glFramebufferTexture2D    = reinterpret_cast<PFNGLFRAMEBUFFERTEXTURE2DPROC>(wglGetProcAddress("glFramebufferTexture2DEXT"));

    if ((NULL != glBindFramebuffer) && (NULL != glDeleteFramebuffers) && (NULL != glGenFramebuffers) && (NULL != glCheckFramebufferStatus) && 
        (NULL != glFramebufferTexture2D))
    {
      Device::SetRenderCapability(EGEDevice::RENDER_CAPS_FBO, true);
    }
  }

  // check if combine texture environment mode is supported
  if (extensionArray.contains("GL_ARB_texture_env_combine"))
  {
    Device::SetRenderCapability(EGEDevice::RENDER_CAPS_COMBINE_TEXTURE_ENV, true);
  }

  // check if vertex buffer object is supported
  // NOTE: this implies that VBO mapping extension is present too
  if (extensionArray.contains("GL_ARB_vertex_buffer_object"))
  {
    glGenBuffers    = reinterpret_cast<PFNGLGENBUFFERSPROC>(wglGetProcAddress("glGenBuffersARB"));
    glBindBuffer    = reinterpret_cast<PFNGLBINDBUFFERPROC>(wglGetProcAddress("glBindBufferARB"));
    glBufferData    = reinterpret_cast<PFNGLBUFFERDATAPROC>(wglGetProcAddress("glBufferDataARB"));
    glBufferSubData = reinterpret_cast<PFNGLBUFFERSUBDATAPROC>(wglGetProcAddress("glBufferSubDataARB"));
    glDeleteBuffers = reinterpret_cast<PFNGLDELETEBUFFERSPROC>(wglGetProcAddress("glDeleteBuffersARB"));
    glMapBuffer     = reinterpret_cast<PFNGLMAPBUFFERPROC>(wglGetProcAddress("glMapBufferARB"));
    glUnmapBuffer   = reinterpret_cast<PFNGLUNMAPBUFFERPROC>(wglGetProcAddress("glUnmapBufferARB"));

    if ((NULL != glGenBuffers) && (NULL != glBindBuffer) && (NULL != glBufferData) && (NULL != glBufferSubData) && (NULL != glDeleteBuffers) && 
        (NULL != glMapBuffer) && (NULL != glUnmapBuffer))
    {
      Device::SetRenderCapability(EGEDevice::RENDER_CAPS_VERTEX_BUFFER_OBJECT, false);
      Device::SetRenderCapability(EGEDevice::RENDER_CAPS_MAP_BUFFER, true);
    }
  }

  // check if point sprites are supported
  if (extensionArray.contains("GL_ARB_point_parameters") && extensionArray.contains("GL_ARB_point_sprite"))
  {
    glPointParameterf  = reinterpret_cast<PFNGLPOINTPARAMETERFARBPROC>(wglGetProcAddress("glPointParameterfARB"));
    glPointParameterfv = reinterpret_cast<PFNGLPOINTPARAMETERFVARBPROC>(wglGetProcAddress("glPointParameterfvARB"));

    if ((NULL != glPointParameterf) && (NULL != glPointParameterfv))
    {
      Device::SetRenderCapability(EGEDevice::RENDER_CAPS_POINT_SPRITE, true);
    }
  }

  // check if min and max blending functions are supported
  if (extensionArray.contains("GL_ARB_imaging") || extensionArray.contains("GL_EXT_blend_minmax"))
  {
    glBlendEquation = reinterpret_cast<PFNGLBLENDEQUATIONPROC>(wglGetProcAddress("glBlendEquationARB"));
    if (NULL == glBlendEquation)
    {
      glBlendEquation = reinterpret_cast<PFNGLBLENDEQUATIONPROC>(wglGetProcAddress("glBlendEquationEXT"));
    }

    if (NULL != glBlendEquation)
    {
      Device::SetRenderCapability(EGEDevice::RENDER_CAPS_BLEND_MINMAX, true);
    }

    glBlendFuncSeparate = reinterpret_cast<PFNGLBLENDFUNCSEPARATEPROC>(wglGetProcAddress("glBlendFuncSeparateEXT"));
  }

  // texture compression
  glCompressedTexImage2D = reinterpret_cast<PFNGLCOMPRESSEDTEXIMAGE2DPROC>(wglGetProcAddress("glCompressedTexImage2D"));

  // check for texture compressions support
  if (extensionArray.contains("GL_IMG_texture_compression_pvrtc"))
  {
    Device::SetRenderCapability(EGEDevice::RENDER_CAPS_TEXTURE_COMPRESSION_PVRTC, true);
  }

  if (extensionArray.contains("GL_EXT_texture_compression_s3tc"))
  {
    Device::SetRenderCapability(EGEDevice::RENDER_CAPS_TEXTURE_COMPRESSION_S3TC, true);
  }

  // check shader objects support
  if (extensionArray.contains("GL_ARB_shader_objects"))
  {
    glCreateShader         = reinterpret_cast<PFNGLCREATESHADERPROC>(wglGetProcAddress("glCreateShader"));
    glDeleteShader         = reinterpret_cast<PFNGLDELETESHADERPROC>(wglGetProcAddress("glDeleteShader"));
    glAttachShader         = reinterpret_cast<PFNGLATTACHSHADERPROC>(wglGetProcAddress("glAttachShader"));
    glDetachShader         = reinterpret_cast<PFNGLDETACHSHADERPROC>(wglGetProcAddress("glDetachShader"));
    glShaderSource         = reinterpret_cast<PFNGLGETSHADERSOURCEPROC>(wglGetProcAddress("glShaderSource"));
    glCompileShader        = reinterpret_cast<PFNGLCOMPILESHADERPROC>(wglGetProcAddress("glCompileShader"));
    glGetShaderiv          = reinterpret_cast<PFNGLGETSHADERIVPROC>(wglGetProcAddress("glGetShaderiv"));
    glGetShaderInfoLog     = reinterpret_cast<PFNGLGETSHADERINFOLOGPROC>(wglGetProcAddress("glGetShaderInfoLog"));
    glCreateProgram        = reinterpret_cast<PFNGLCREATEPROGRAMPROC>(wglGetProcAddress("glCreateProgram"));
    glDeleteProgram        = reinterpret_cast<PFNGLDELETEPROGRAMPROC>(wglGetProcAddress("glDeleteProgram"));
    glLinkProgram          = reinterpret_cast<PFNGLLINKPROGRAMARBPROC>(wglGetProcAddress("glLinkProgram"));
    glValidateProgram      = reinterpret_cast<PFNGLVALIDATEPROGRAMARBPROC>(wglGetProcAddress("glValidateProgram"));
    glUseProgram           = reinterpret_cast<PFNGLUSEPROGRAMPROC>(wglGetProcAddress("glUseProgram"));
    glGetProgramiv         = reinterpret_cast<PFNGLGETPROGRAMIVPROC>(wglGetProcAddress("glGetProgramiv"));
    glGetProgramInfoLog    = reinterpret_cast<PFNGLGETPROGRAMINFOLOGPROC>(wglGetProcAddress("glGetProgramInfoLog"));
    glGetUniformLocation   = reinterpret_cast<PFNGLGETUNIFORMLOCATIONPROC>(wglGetProcAddress("glGetUniformLocation"));
    glGetActiveUniform     = reinterpret_cast<PFNGLGETACTIVEUNIFORMPROC>(wglGetProcAddress("glGetActiveUniform"));
    glUniform1f            = reinterpret_cast<PFNGLUNIFORM1FARBPROC>(wglGetProcAddress("glUniform1f"));
    glUniform2f            = reinterpret_cast<PFNGLUNIFORM2FARBPROC>(wglGetProcAddress("glUniform2f"));
    glUniform3f            = reinterpret_cast<PFNGLUNIFORM3FARBPROC>(wglGetProcAddress("glUniform3f"));
    glUniform4f            = reinterpret_cast<PFNGLUNIFORM4FARBPROC>(wglGetProcAddress("glUniform4f"));
    glUniform1i            = reinterpret_cast<PFNGLUNIFORM1IARBPROC>(wglGetProcAddress("glUniform1i"));
    glUniform2i            = reinterpret_cast<PFNGLUNIFORM2IARBPROC>(wglGetProcAddress("glUniform2i"));
    glUniform3i            = reinterpret_cast<PFNGLUNIFORM3IARBPROC>(wglGetProcAddress("glUniform3i"));
    glUniform4i            = reinterpret_cast<PFNGLUNIFORM4IARBPROC>(wglGetProcAddress("glUniform4i"));
    glUniformMatrix2fv     = reinterpret_cast<PFNGLUNIFORMMATRIX2FVARBPROC>(wglGetProcAddress("glUniformMatrix2fvARB"));
    glUniformMatrix3fv     = reinterpret_cast<PFNGLUNIFORMMATRIX3FVARBPROC>(wglGetProcAddress("glUniformMatrix3fvARB"));
    glUniformMatrix4fv     = reinterpret_cast<PFNGLUNIFORMMATRIX4FVARBPROC>(wglGetProcAddress("glUniformMatrix4fvARB"));
  }

  // check vertex shader support
  if (extensionArray.contains("GL_ARB_vertex_shader"))
  {
    glDisableVertexAttribArray  = reinterpret_cast<PFNGLDISABLEVERTEXATTRIBARRAYPROC>(wglGetProcAddress("glDisableVertexAttribArray"));
    glEnableVertexAttribArray   = reinterpret_cast<PFNGLENABLEVERTEXATTRIBARRAYPROC>(wglGetProcAddress("glEnableVertexAttribArray"));
    glGetAttribLocation         = reinterpret_cast<PFNGLGETATTRIBLOCATIONPROC>(wglGetProcAddress("glGetAttribLocation"));
    glVertexAttribPointer       = reinterpret_cast<PFNGLVERTEXATTRIBPOINTERARBPROC>(wglGetProcAddress("glVertexAttribPointer"));
    glGetActiveAttrib           = reinterpret_cast<PFNGLGETACTIVEATTRIBPROC>(wglGetProcAddress("glGetActiveAttrib"));

    if ((NULL != glDisableVertexAttribArray) && (NULL != glEnableVertexAttribArray) && (NULL != glGetAttribLocation) && 
        (NULL != glVertexAttribPointer) && (NULL != glGetActiveAttrib))
    {
      Device::SetRenderCapability(EGEDevice::RENDER_CAPS_VERTEX_SHADER, true);
    }
  }

  // check fragment shader support
  if (extensionArray.contains("GL_ARB_fragment_shader"))
  {
    Device::SetRenderCapability(EGEDevice::RENDER_CAPS_FRAGMENT_SHADER, true);
  }
  
  // check vertex array objects support
  if (extensionArray.contains("GL_ARB_vertex_array_object"))
  {
    glBindVertexArray     = reinterpret_cast<PFNGLBINDVERTEXARRAYARBPROC>(wglGetProcAddress("glBindVertexArray"));
    glDeleteVertexArrays  = reinterpret_cast<PFNGLDELETEVERTEXARRAYSARBPROC>(wglGetProcAddress("glDeleteVertexArrays"));
    glGenVertexArrays     = reinterpret_cast<PFNGLGENVERTEXARRAYSARBPROC>(wglGetProcAddress("glGenVertexArrays"));
    glIsVertexArray       = reinterpret_cast<PFNGLISVERTEXARRAYARBPROC>(wglGetProcAddress("glIsVertexArray"));

    if ((NULL != glIsVertexArray) && (NULL != glGenVertexArrays) && (NULL != glDeleteVertexArrays) && (NULL != glBindVertexArray))
    {
      Device::SetRenderCapability(EGEDevice::RENDER_CAPS_VERTEX_ARRAY_OBJECT, true);
    }
  }

  // Point sprite size array is not supported by default
  Device::SetRenderCapability(EGEDevice::RENDER_CAPS_POINT_SPRITE_SIZE, false);

  // 32bit indexing is supported by default
  Device::SetRenderCapability(EGEDevice::RENDER_CAPS_ELEMENT_INDEX_UINT, true);

  // at least one check at the end
  OGL_CHECK();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool RenderWindowOGLWin32::isAutoRotated() const
{
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END