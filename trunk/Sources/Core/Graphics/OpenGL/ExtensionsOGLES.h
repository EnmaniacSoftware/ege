#ifndef EGE_CORE_EXTENSIONSOGLES_H
#define EGE_CORE_EXTENSIONSOGLES_H

#include <EGE.h>
#include <GLES/egl.h>
#include <GLES/glext.h>

EGE_NAMESPACE_BEGIN

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

// Texture Compression S3TC
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT   0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT  0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT  0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT  0x83F3

#define GL_WRITE_ONLY GL_WRITE_ONLY_OES

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_EXTENSIONSOGLES_H