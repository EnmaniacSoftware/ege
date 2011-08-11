#include "Core/Graphics/OpenGL/ExtensionsOGL.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// multitexturing
PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTexture = NULL;
PFNGLACTIVETEXTUREARBPROC glActiveTexture = NULL;

// frame buffer object
PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffers = NULL;
PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffers = NULL;
PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebuffer = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatus = NULL;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2D = NULL;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
