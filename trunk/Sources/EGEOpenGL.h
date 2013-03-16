#ifndef EGE_OPENGL_H
#define EGE_OPENGL_H

#include "Core/Platform.h"

#ifdef EGE_PLATFORM_WIN32
  #include <windows.h>
  #include <gl/gl.h>
  #include "Core/Graphics/OpenGL/ExtensionsOGL.h"
#elif EGE_PLATFORM_AIRPLAY
  #include <GLES/gl.h>
  #include "Core/Graphics/OpenGL/ExtensionsOGL.h"
#elif EGE_PLATFORM_IOS
  #import <OpenGLES/ES1/gl.h>
  #include "Core/Graphics/OpenGL/ExtensionsOGL.h"
#endif

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef EGE_FEATURE_OPENGL_DEBUG
  #define OGL_CHECK() if (GL_NO_ERROR != glGetError()) { egeCritical() << "OpenGL error" << __FILE__ << __LINE__; }
  #define OGL_CHECK_RESULT(result) if (GL_NO_ERROR != (result = glGetError())) { egeCritical() << "OpenGL error" << __FILE__ << __LINE__; }
#else
  #define OGL_CHECK() ege_noop()
  #define OGL_CHECK_RESULT(result) ege_noop()
#endif // EGE_FEATURE_OPENGL_DEBUG
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_OPENGL_H