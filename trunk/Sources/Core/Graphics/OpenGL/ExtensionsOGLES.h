#ifndef EGE_CORE_EXTENSIONSOGLES_H
#define EGE_CORE_EXTENSIONSOGLES_H

#include "EGE.h"
#include "EGEOpenGL.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

// GL_OES_framebuffer_object
typedef void (GL_APIENTRY* PFNGLBINDFRAMEBUFFEROESPROC) (GLenum target, GLuint framebuffer);
typedef void (GL_APIENTRY* PFNGLDELETEFRAMEBUFFERSOESPROC) (GLsizei n, const GLuint* framebuffers);
typedef void (GL_APIENTRY* PFNGLGENFRAMEBUFFERSOESPROC) (GLsizei n, GLuint* framebuffers);
typedef GLenum (GL_APIENTRY* PFNGLCHECKFRAMEBUFFERSTATUSOESPROC) (GLenum target);
typedef void (GL_APIENTRY* PFNGLFRAMEBUFFERTEXTURE2DOESPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);

extern PFNGLBINDFRAMEBUFFEROESPROC glBindFramebuffer;
extern PFNGLDELETEFRAMEBUFFERSOESPROC glDeleteFramebuffers;
extern PFNGLGENFRAMEBUFFERSOESPROC glGenFramebuffers;
extern PFNGLCHECKFRAMEBUFFERSTATUSOESPROC glCheckFramebufferStatus;
extern PFNGLFRAMEBUFFERTEXTURE2DOESPROC glFramebufferTexture2D;

#define GL_FRAMEBUFFER_EXT          GL_FRAMEBUFFER_OES
#define GL_COLOR_ATTACHMENT0_EXT    GL_COLOR_ATTACHMENT0_OES
#define GL_FRAMEBUFFER_BINDING_EXT  GL_FRAMEBUFFER_BINDING_OES
#define GL_FRAMEBUFFER_COMPLETE_EXT GL_FRAMEBUFFER_COMPLETE_OES

// GL_OES_point_sprite
#define GL_POINT_SPRITE_ARB     GL_POINT_SPRITE_OES
#define GL_COORD_REPLACE_ARB    GL_COORD_REPLACE_OES
#define GL_POINT_SIZE_ARRAY_ARB GL_POINT_SIZE_ARRAY_OES

// GL_OES_mapbuffer
typedef void* (GL_APIENTRY* PFNGLMAPBUFFEROESPROC) (GLenum target, GLenum access);
typedef GLboolean (GL_APIENTRY* PFNGLUNMAPBUFFEROESPROC) (GLenum target);

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