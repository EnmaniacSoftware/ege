#ifndef EGE_CORE_EXTENSIONSOGL_H
#define EGE_CORE_EXTENSIONSOGL_H

#include <EGE.h>
#include <EGEOpenGL.h>
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

// vertex buffer object
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLMAPBUFFERPROC glMapBuffer;
extern PFNGLUNMAPBUFFERPROC glUnmapBuffer;

// point sprites
extern PFNGLPOINTPARAMETERFARBPROC  glPointParameterf;
extern PFNGLPOINTPARAMETERFVARBPROC glPointParameterfv;

// blend equations
extern PFNGLBLENDEQUATIONPROC glBlendEquation;
extern PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate;

// texture compression
extern PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;

// texture compression PVRTC
#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG			0x8C00
#define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG			0x8C01
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG			0x8C02
#define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG			0x8C03
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_CORE_EXTENSIONSOGL_H