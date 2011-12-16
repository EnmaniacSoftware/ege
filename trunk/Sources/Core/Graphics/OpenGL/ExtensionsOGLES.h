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

#define GL_FRAMEBUFFER_EXT          GL_FRAMEBUFFER_OES
#define GL_COLOR_ATTACHMENT0_EXT    GL_COLOR_ATTACHMENT0_OES
#define GL_FRAMEBUFFER_BINDING_EXT  GL_FRAMEBUFFER_BINDING_OES
#define GL_FRAMEBUFFER_COMPLETE_EXT GL_FRAMEBUFFER_COMPLETE_OES

// point sprites
#define GL_POINT_SPRITE_ARB     GL_POINT_SPRITE_OES
#define GL_COORD_REPLACE_ARB    GL_COORD_REPLACE_OES
#define GL_POINT_SIZE_ARRAY_ARB GL_POINT_SIZE_ARRAY_OES

// map buffer
extern PFNGLMAPBUFFEROESPROC glMapBuffer;
extern PFNGLUNMAPBUFFEROESPROC glUnmapBuffer;

#define GL_WRITE_ONLY GL_WRITE_ONLY_OES

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_CORE_EXTENSIONSOGLES_H