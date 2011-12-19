#ifndef EGE_OPENGL_H
#define EGE_OPENGL_H

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "Core/Platform.h"

#ifdef EGE_PLATFORM_WIN32
#include <gl/GL.h>
#else
#include <GLES/gl.h>
#endif

#if EGE_RENDERING_OPENGL_2
#include "Core/Graphics/OpenGL/ExtensionsOGL.h"
#elif EGE_RENDERING_OPENGLES_1
#include "Core/Graphics/OpenGL/ExtensionsOGLES.h"
#endif // EGE_RENDERING_OPENGLES_1

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_OPENGL_H