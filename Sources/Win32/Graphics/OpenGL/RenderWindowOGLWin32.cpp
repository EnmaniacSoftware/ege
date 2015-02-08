#include "Core/Engine/Interface/EngineInstance.h"
#include "Win32/Graphics/OpenGL/RenderWindowOGLWin32.h"
#include "EGEOpenGL.h"
#include "EGEEvent.h"
#include "EGEInput.h"
#include "EGEMath.h"
#include "EGEDevice.h"
#include "EGEEngine.h"
#include "EGEMap.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local helper function for mapping Windows virtual keys into Framework equivalents. 
 *  @param  virtualKey  Windows virtual key code to map.
 *  @return EGE framework equivalent code. EKeyNone is returned if no conversion could be made.
 */
static Key MapVirtualKeyToFramework(DWORD virtualKey)
{
  static Map<DWORD, Key> mapping;
  
  // check if not filled yet
  if (mapping.empty())
  {
    mapping[VK_BACK]   = EKeyBackspace;
    mapping[VK_TAB]    = EKeyTab;
    mapping[VK_RETURN] = EKeyEnter;
    mapping[VK_ESCAPE] = EKeyEsc;
    mapping[VK_SPACE]  = EKeySpace;
    mapping[VK_PRIOR]  = EKeyPageUp;
    mapping[VK_NEXT]   = EKeyPageDown;
    mapping[VK_END]    = EKeyEnd;
    mapping[VK_INSERT] = EKeyInsert;
    mapping[VK_DELETE] = EKeyDelete;

    mapping[VK_LEFT]  = EKeyArrowLeft;
    mapping[VK_UP]    = EKeyArrowUp;
    mapping[VK_RIGHT] = EKeyArrowRight;
    mapping[VK_DOWN]  = EKeyArrowDown;

    mapping['0'] = EKey0;
    mapping['1'] = EKey1;
    mapping['2'] = EKey2;
    mapping['3'] = EKey3;
    mapping['4'] = EKey4;
    mapping['5'] = EKey5;
    mapping['6'] = EKey6;
    mapping['7'] = EKey7;
    mapping['8'] = EKey8;
    mapping['9'] = EKey9;

    mapping['A'] = EKeyA;
    mapping['B'] = EKeyB;
    mapping['C'] = EKeyC;
    mapping['D'] = EKeyD;
    mapping['E'] = EKeyE;
    mapping['F'] = EKeyF;
    mapping['G'] = EKeyG;
    mapping['H'] = EKeyH;
    mapping['I'] = EKeyI;
    mapping['J'] = EKeyJ;
    mapping['K'] = EKeyK;
    mapping['L'] = EKeyL;
    mapping['M'] = EKeyM;
    mapping['N'] = EKeyN;
    mapping['O'] = EKeyO;
    mapping['P'] = EKeyP;
    mapping['Q'] = EKeyQ;
    mapping['R'] = EKeyR;
    mapping['S'] = EKeyS;
    mapping['T'] = EKeyT;
    mapping['U'] = EKeyU;
    mapping['V'] = EKeyV;
    mapping['W'] = EKeyW;
    mapping['X'] = EKeyX;
    mapping['Y'] = EKeyY;
    mapping['Z'] = EKeyZ;

    mapping[VK_NUMLOCK] = EKeyNumLock;
    mapping[VK_NUMPAD0]  = EKeyNumpad0;
    mapping[VK_NUMPAD1]  = EKeyNumpad1;
    mapping[VK_NUMPAD2]  = EKeyNumpad2;
    mapping[VK_NUMPAD3]  = EKeyNumpad3;
    mapping[VK_NUMPAD4]  = EKeyNumpad4;
    mapping[VK_NUMPAD5]  = EKeyNumpad5;
    mapping[VK_NUMPAD6]  = EKeyNumpad6;
    mapping[VK_NUMPAD7]  = EKeyNumpad7;
    mapping[VK_NUMPAD8]  = EKeyNumpad8;
    mapping[VK_NUMPAD9]  = EKeyNumpad9;
    mapping[VK_MULTIPLY] = EKeyNumpadMultiple;
    mapping[VK_ADD]      = EKeyNumpadAdd;
    mapping[VK_SUBTRACT] = EKeyNumpadSubtract;
    mapping[VK_DECIMAL]  = EKeyNumpadDecimal;
    mapping[VK_DIVIDE]   = EKeyNumpadDivide;

    mapping[VK_F1]  = EKeyF1;
    mapping[VK_F2]  = EKeyF2;
    mapping[VK_F3]  = EKeyF3;
    mapping[VK_F4]  = EKeyF4;
    mapping[VK_F5]  = EKeyF5;
    mapping[VK_F6]  = EKeyF6;
    mapping[VK_F7]  = EKeyF7;
    mapping[VK_F8]  = EKeyF8;
    mapping[VK_F9]  = EKeyF9;
    mapping[VK_F10] = EKeyF10;
    mapping[VK_F11] = EKeyF11;
    mapping[VK_F12] = EKeyF12;

    mapping[VK_SHIFT]   = EKeyLeftShift;
    mapping[VK_CONTROL] = EKeyLeftCtrl;
    mapping[VK_MENU]    = EKeyLeftAlt;

    mapping[VK_OEM_1]      = EKeySemicolon;
    mapping[VK_OEM_PLUS]   = EKeyEquals;
    mapping[VK_OEM_COMMA]  = EKeyComma;
    mapping[VK_OEM_MINUS]  = EKeyMinus;
    mapping[VK_OEM_PERIOD] = EKeyPeriod;
    mapping[VK_OEM_2]      = EKeySlash;
    mapping[VK_OEM_3]      = EKeyTilde;
    mapping[VK_OEM_4]      = EKeyLeftBracket;
    mapping[VK_OEM_5]      = EKeyBackslash;
    mapping[VK_OEM_6]      = EKeyRightBracket;
    mapping[VK_OEM_7]      = EKeyApostrophe;
  }

  // do mapping
  Key key = mapping.value(virtualKey, EKeyNone);

  // check if one of the control keys
  // NOTE: for control keys we need further investigation to determine exact key (left or right)
  if ((EKeyLeftShift == key) && (GetAsyncKeyState(VK_RSHIFT) & 0x8000))
  {
    key = EKeyRightShift;
  }
  else if ((EKeyLeftCtrl == key) && (GetAsyncKeyState(VK_RCONTROL) & 0x8000))
  {
    key = EKeyRightCtrl;
  }
  else if ((EKeyLeftAlt == key) && (GetAsyncKeyState(VK_RMENU) & 0x8000))
  {
    key = EKeyRightAlt;
  }

  return key;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderWindowOGLWin32::RenderWindowOGLWin32(Engine& engine, const Dictionary& params) 
: RenderWindow(params)
, m_engine(engine)
, m_hWnd(NULL)
, m_hDC(NULL)
, m_hRC(NULL)
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
  if (NULL == (m_hWnd = CreateWindowA("RenderWindowOGLWin32ClassName", title().toAscii(), dwStyles, 0, 0, rcWindow.right - rcWindow.left, 
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

  EngineInstance* engineInstance = NULL;
  IEventManager* eventManager = NULL;

  if (NULL != me)
  {
    engineInstance = reinterpret_cast<EngineInstance*>(&me->engine());
    eventManager = me->engine().eventManager();
  }

  // detrmine current keyboard modifiers
  KeyboardModifiers keyboardModifiers = EKeyModifierNone;
  if (GetAsyncKeyState(VK_MENU) & 0x8000)
  {
    keyboardModifiers |= EKeyModifierAlt;
  }

  if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
  {
    keyboardModifiers |= EKeyModifierCtrl;
  }

  if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
  {
    keyboardModifiers |= EKeyModifierShift;
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

        PObject data = ege_new MouseEvent(EPointerActionDown, EMouseButtonLeft, keyboardModifiers, x, y);
        eventManager->send(EGE_EVENT_ID_CORE_INPUT_EVENT, data);
      }
      return 0;

    case WM_LBUTTONUP:

      if (NULL != eventManager)
      {
        x = static_cast<s32>(LOWORD(lParam) / me->zoom());
        y = static_cast<s32>(HIWORD(lParam) / me->zoom());

        PObject data = ege_new MouseEvent(EPointerActionUp, EMouseButtonLeft, keyboardModifiers, x, y);
        eventManager->send(EGE_EVENT_ID_CORE_INPUT_EVENT, ege_pcast<PObject>(data));
      }
      return 0;

    case WM_RBUTTONDOWN:

      if (NULL != eventManager)
      {
        x = static_cast<s32>(LOWORD(lParam) / me->zoom());
        y = static_cast<s32>(HIWORD(lParam) / me->zoom());

        PObject data = ege_new MouseEvent(EPointerActionDown, EMouseButtonRight, keyboardModifiers, x, y);
        eventManager->send(EGE_EVENT_ID_CORE_INPUT_EVENT, data);
      }
      return 0;

    case WM_RBUTTONUP:

      if (NULL != eventManager)
      {
        x = static_cast<s32>(LOWORD(lParam) / me->zoom());
        y = static_cast<s32>(HIWORD(lParam) / me->zoom());

        PObject data = ege_new MouseEvent(EPointerActionUp, EMouseButtonRight, keyboardModifiers, x, y);
        eventManager->send(EGE_EVENT_ID_CORE_INPUT_EVENT, data);
      }
      return 0;

    case WM_MOUSEMOVE:

      if (NULL != eventManager)
      {
        x = static_cast<s32>(LOWORD(lParam) / me->zoom());
        y = static_cast<s32>(HIWORD(lParam) / me->zoom());

        PObject data = ege_new MouseEvent(EPointerActionMove, EMouseButtonNone, keyboardModifiers, x, y);
        eventManager->send(EGE_EVENT_ID_CORE_INPUT_EVENT, data);
      }
      return 0;

    case WM_SIZE:

 //     ::engine->resizeWindow( LOWORD( lParam ), HIWORD( lParam ) );
      return 0;

    case WM_MOUSEWHEEL:

      if (NULL != eventManager)
      {
        const short delta = GET_WHEEL_DELTA_WPARAM(wParam);

        x = static_cast<s32>(LOWORD(lParam) / me->zoom());
        y = static_cast<s32>(HIWORD(lParam) / me->zoom());

        PObject data = ege_new MouseEvent(EPointerActionMove, (0 < delta) ? EMouseButtonWheelUp : EMouseButtonWheelDown, keyboardModifiers, x, y);
        eventManager->send(EGE_EVENT_ID_CORE_INPUT_EVENT, data);
      }
      return 0;

    case WM_KEYDOWN:
    case WM_KEYUP:

      if (NULL != eventManager)
      {
        // map the key
        const Key key = MapVirtualKeyToFramework(wParam);
        
        // check if any valid key been pressed
        if (EKeyNone != key)
        {
          // send event
          PObject data = ege_new KeyEvent(key, (WM_KEYDOWN == msg) ? EKeyActionDown : EKeyActionUp, keyboardModifiers);
          eventManager->send(EGE_EVENT_ID_CORE_INPUT_EVENT, data);
        }
      }
      return 0;

    case WM_CLOSE:
    case WM_QUIT:

      // shut down engine
      if (NULL != me)
      {
        eventManager->send(EGE_EVENT_ID_CORE_QUIT_REQUEST);
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
  if ( ! extensionString.isEmpty())
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
      Device::SetRenderCapability(ERenderCapabilityMultitexturing, true);
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
    glGenerateMipmap          = reinterpret_cast<PFNGLGENERATEMIPMAPPROC>(wglGetProcAddress("glGenerateMipmapEXT"));

    if ((NULL != glBindFramebuffer) && (NULL != glDeleteFramebuffers) && (NULL != glGenFramebuffers) && (NULL != glCheckFramebufferStatus) && 
        (NULL != glFramebufferTexture2D) && (NULL != glGenerateMipmap))
    {
      Device::SetRenderCapability(ERenderCapabilityFrameBufferObjects, true);
    }
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
      Device::SetRenderCapability(ERenderCapabilityVertexBufferObjects, false);
      Device::SetRenderCapability(ERenderCapabilityMapBuffer, true);
    }
  }

  // check if point sprites are supported
  if (extensionArray.contains("GL_ARB_point_parameters") && extensionArray.contains("GL_ARB_point_sprite"))
  {
    glPointParameterf  = reinterpret_cast<PFNGLPOINTPARAMETERFARBPROC>(wglGetProcAddress("glPointParameterfARB"));
    glPointParameterfv = reinterpret_cast<PFNGLPOINTPARAMETERFVARBPROC>(wglGetProcAddress("glPointParameterfvARB"));

    if ((NULL != glPointParameterf) && (NULL != glPointParameterfv))
    {
      Device::SetRenderCapability(ERenderCapabilityPointSprites, true);
    }
  }

  // check if min and max blending functions are supported
  if (extensionArray.contains("GL_ARB_imaging"))
  {
    glBlendEquation = reinterpret_cast<PFNGLBLENDEQUATIONPROC>(wglGetProcAddress("glBlendEquationARB"));
    if (NULL == glBlendEquation)
    {
      glBlendEquation = reinterpret_cast<PFNGLBLENDEQUATIONPROC>(wglGetProcAddress("glBlendEquationEXT"));
    }

    glBlendFuncSeparate = reinterpret_cast<PFNGLBLENDFUNCSEPARATEPROC>(wglGetProcAddress("glBlendFuncSeparateEXT"));
  }

  // texture compression
  glCompressedTexImage2D = reinterpret_cast<PFNGLCOMPRESSEDTEXIMAGE2DPROC>(wglGetProcAddress("glCompressedTexImage2D"));

  // check for texture compressions support
  Device::SetRenderCapability(ERenderCapabilityTextureCompressionPVRTC, extensionArray.contains("GL_IMG_texture_compression_pvrtc"));
  Device::SetRenderCapability(ERenderCapabilityTextureCompressionS3TC, extensionArray.contains("GL_EXT_texture_compression_s3tc"));

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
      Device::SetRenderCapability(ERenderCapabilityVertexShaders, true);
    }
  }

  // check fragment shader support
  Device::SetRenderCapability(ERenderCapabilityFragmentShaders, extensionArray.contains("GL_ARB_fragment_shader"));
  
  // check vertex array objects support
  if (extensionArray.contains("GL_ARB_vertex_array_object"))
  {
    glBindVertexArray     = reinterpret_cast<PFNGLBINDVERTEXARRAYARBPROC>(wglGetProcAddress("glBindVertexArray"));
    glDeleteVertexArrays  = reinterpret_cast<PFNGLDELETEVERTEXARRAYSARBPROC>(wglGetProcAddress("glDeleteVertexArrays"));
    glGenVertexArrays     = reinterpret_cast<PFNGLGENVERTEXARRAYSARBPROC>(wglGetProcAddress("glGenVertexArrays"));
    glIsVertexArray       = reinterpret_cast<PFNGLISVERTEXARRAYARBPROC>(wglGetProcAddress("glIsVertexArray"));

    if ((NULL != glIsVertexArray) && (NULL != glGenVertexArrays) && (NULL != glDeleteVertexArrays) && (NULL != glBindVertexArray))
    {
      Device::SetRenderCapability(ERenderCapabilityVertexArrayObjects, true);
    }
  }

  // 32bit indexing is supported by default
  Device::SetRenderCapability(ERenderCapabilityElementIndexUnsignedInt, true);

  // auto mipmapping support
  Device::SetRenderCapability(ERenderCapabilityAutoMipmapping, extensionArray.contains("GL_SGIS_generate_mipmap"));

  // at least one check at the end
  OGL_CHECK()
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool RenderWindowOGLWin32::isAutoRotated() const
{
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Engine& RenderWindowOGLWin32::engine() const
{
  return m_engine;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END