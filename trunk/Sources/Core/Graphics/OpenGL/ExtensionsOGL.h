#ifndef EGE_CORE_EXTENSIONSOGL_H
#define EGE_CORE_EXTENSIONSOGL_H

#include <EGE.h>
#include <gl/GL.h>
#include "Core/Graphics/OpenGL/GL 2.0/glext.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

// multitexturing
extern PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTexture;
extern PFNGLACTIVETEXTUREARBPROC glActiveTexture;

// frame buffer object
extern PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffers;
extern PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffers;
extern PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebuffer;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatus;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2D;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_CORE_EXTENSIONSOGL_H