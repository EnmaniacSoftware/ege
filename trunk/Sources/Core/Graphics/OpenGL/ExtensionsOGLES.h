#ifndef EGE_CORE_EXTENSIONSOGLES_H
#define EGE_CORE_EXTENSIONSOGLES_H

#include <EGE.h>
#include <GLES/egl.h>
#include <GLES/glext.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

// frame buffer object
extern PFNGLBINDFRAMEBUFFEROESPROC glBindFramebuffer;
extern PFNGLDELETEFRAMEBUFFERSOESPROC glDeleteFramebuffers;
extern PFNGLGENFRAMEBUFFERSOESPROC glGenFramebuffers;
extern PFNGLCHECKFRAMEBUFFERSTATUSOESPROC glCheckFramebufferStatus;
extern PFNGLFRAMEBUFFERTEXTURE2DOESPROC glFramebufferTexture2D;

#define GL_FRAMEBUFFER_EXT       GL_FRAMEBUFFER_OES
#define GL_COLOR_ATTACHMENT0_EXT GL_COLOR_ATTACHMENT0_OES

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_CORE_EXTENSIONSOGLES_H