#include "Core/Graphics/OpenGL/ExtensionsOGLES.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// frame buffer object
PFNGLBINDFRAMEBUFFEROESPROC glBindFramebuffer = NULL;
PFNGLDELETEFRAMEBUFFERSOESPROC glDeleteFramebuffers = NULL;
PFNGLGENFRAMEBUFFERSOESPROC glGenFramebuffers = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSOESPROC glCheckFramebufferStatus = NULL;
PFNGLFRAMEBUFFERTEXTURE2DOESPROC glFramebufferTexture2D = NULL;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
