﻿#ifndef EGE_IOS_GRAPHICS_OPENGL_EXTENSIONSOGL_H
#define EGE_IOS_GRAPHICS_OPENGL_EXTENSIONSOGL_H

#include "EGE.h"
#include "EGEOpenGL.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef GL_APIENTRYP
  #define GL_APIENTRYP *
#endif // GL_APIENTRYP
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

// Stub for unsupported features in OGLES
#define GL_TEXTURE_1D         0x0DE0
#define GL_TEXTURE_BINDING_1D 0x8068

// GL_EXT_texture_compression_s3tc
#define GL_COMPRESSED_RGB_S3TC_DXT1   0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1  0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3  0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5  0x83F3

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef const GLubyte* (GL_APIENTRYP PFNGLGETSTRINGIPROC) (GLenum name, GLuint index);
extern PFNGLGETSTRINGIPROC glGetStringi;

// GL_ARB_point_sprite
#define GL_POINT_SPRITE   0x8861
#define GL_COORD_REPLACE  0x8862

// GL_EXT_framebuffer_object
#define GL_FRAMEBUFFER_BINDING  0x8CA6
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#define GL_COLOR_ATTACHMENT0    0x8CE0
#define GL_DEPTH_ATTACHMENT     0x8D00
#define GL_FRAMEBUFFER          0x8D40
#define GL_DEPTH_COMPONENT16    0x81A5

typedef void (GL_APIENTRYP PFNGLGENFRAMEBUFFERSPROC) (GLsizei n, GLuint *framebuffers);
typedef void (GL_APIENTRYP PFNGLDELETEFRAMEBUFFERSPROC) (GLsizei n, const GLuint *framebuffers);
typedef void (GL_APIENTRYP PFNGLBINDFRAMEBUFFERPROC) (GLenum target, GLuint framebuffer);
typedef GLenum (GL_APIENTRYP PFNGLCHECKFRAMEBUFFERSTATUSPROC) (GLenum target);
typedef void (GL_APIENTRYP PFNGLFRAMEBUFFERTEXTURE2DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef GLvoid (GL_APIENTRYP PFNGLRENDERBUFFERSTORAGEPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GL_APIENTRYP PFNGLGENERATEMIPMAPPROC) (GLenum target);

extern PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
extern PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
extern PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
extern PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;

// GL_OES_mapbuffer
typedef GLvoid* (GL_APIENTRYP PFNGLMAPBUFFERPROC) (GLenum target, GLenum access);
typedef GLboolean (GL_APIENTRYP PFNGLUNMAPBUFFERPROC) (GLenum target);

extern PFNGLMAPBUFFERPROC glMapBuffer;
extern PFNGLUNMAPBUFFERPROC glUnmapBuffer;

// blend equations
typedef void (GL_APIENTRYP PFNGLBLENDEQUATIONPROC) (GLenum mode);
typedef void (GL_APIENTRYP PFNGLBLENDFUNCSEPARATEPROC) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);

extern PFNGLBLENDEQUATIONPROC glBlendEquation;
extern PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate;

// texture compression PVRTC
#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG			0x8C00
#define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG			0x8C01
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG			0x8C02
#define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG			0x8C03

// GL_ARB_shader_objects
#define GL_COMPILE_STATUS    0x8B81
#define GL_LINK_STATUS       0x8B82
#define GL_INFO_LOG_LENGTH   0x8B84
#define GL_ACTIVE_UNIFORMS   0x8B86
#define GL_ACTIVE_ATTRIBUTES 0x8B89
#define GL_CURRENT_PROGRAM   0x8B8D

typedef char GLchar;
typedef GLuint GLhandle;

typedef GLhandle (GL_APIENTRYP PFNGLCREATESHADERPROC) (GLenum shaderType);
typedef void (GL_APIENTRYP PFNGLDELETESHADERPROC) (GLhandle obj);
typedef void (GL_APIENTRYP PFNGLDETACHSHADERPROC) (GLhandle containerObj, GLhandle attachedObj);
typedef void (GL_APIENTRYP PFNGLGETSHADERSOURCEPROC) (GLhandle shaderObj, GLsizei count, const GLchar* const *string, const GLint *length);
typedef void (GL_APIENTRYP PFNGLCOMPILESHADERPROC) (GLhandle shaderObj);
typedef void (GL_APIENTRYP PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint* params);
typedef GLhandle (GL_APIENTRYP PFNGLCREATEPROGRAMPROC) (void);
typedef void (GL_APIENTRYP PFNGLDELETEPROGRAMPROC) (GLhandle program);
typedef void (GL_APIENTRYP PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint *params);
typedef void (GL_APIENTRYP PFNGLATTACHSHADERPROC) (GLhandle containerObj, GLhandle obj);
typedef void (GL_APIENTRYP PFNGLLINKPROGRAMARBPROC) (GLhandle programObj);
typedef void (GL_APIENTRYP PFNGLVALIDATEPROGRAMARBPROC) (GLhandle programObj);
typedef void (GL_APIENTRYP PFNGLUSEPROGRAMPROC) (GLhandle programObj);
typedef GLint (GL_APIENTRYP PFNGLGETUNIFORMLOCATIONPROC) (GLhandle programObj, const GLchar*name);
typedef void (GL_APIENTRYP PFNGLGETACTIVEUNIFORMPROC) (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
typedef void (GL_APIENTRYP PFNGLUNIFORM1FARBPROC) (GLint location, GLfloat v0);
typedef void (GL_APIENTRYP PFNGLUNIFORM2FARBPROC) (GLint location, GLfloat v0, GLfloat v1);
typedef void (GL_APIENTRYP PFNGLUNIFORM3FARBPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (GL_APIENTRYP PFNGLUNIFORM4FARBPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (GL_APIENTRYP PFNGLUNIFORM1IARBPROC) (GLint location, GLint v0);
typedef void (GL_APIENTRYP PFNGLUNIFORM2IARBPROC) (GLint location, GLint v0, GLint v1);
typedef void (GL_APIENTRYP PFNGLUNIFORM3IARBPROC) (GLint location, GLint v0, GLint v1, GLint v2);
typedef void (GL_APIENTRYP PFNGLUNIFORM4IARBPROC) (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (GL_APIENTRYP PFNGLUNIFORMMATRIX2FVARBPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GL_APIENTRYP PFNGLUNIFORMMATRIX3FVARBPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GL_APIENTRYP PFNGLUNIFORMMATRIX4FVARBPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLDETACHSHADERPROC glDetachShader;
extern PFNGLGETSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLVALIDATEPROGRAMARBPROC glValidateProgram;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLLINKPROGRAMARBPROC glLinkProgram;
extern PFNGLUSEPROGRAMPROC glUseProgram;

extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform;
extern PFNGLUNIFORM1FARBPROC glUniform1f;
extern PFNGLUNIFORM2FARBPROC glUniform2f;
extern PFNGLUNIFORM3FARBPROC glUniform3f;
extern PFNGLUNIFORM4FARBPROC glUniform4f;
extern PFNGLUNIFORM1IARBPROC glUniform1i;
extern PFNGLUNIFORM2IARBPROC glUniform2i;
extern PFNGLUNIFORM3IARBPROC glUniform3i;
extern PFNGLUNIFORM4IARBPROC glUniform4i;
extern PFNGLUNIFORMMATRIX2FVARBPROC glUniformMatrix2fv;
extern PFNGLUNIFORMMATRIX3FVARBPROC glUniformMatrix3fv;
extern PFNGLUNIFORMMATRIX4FVARBPROC glUniformMatrix4fv;

// ARB_vertex_shader
#define GL_VERTEX_SHADER 0x8B31

typedef void (GL_APIENTRYP PFNGLDISABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (GL_APIENTRYP PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef GLint (GL_APIENTRYP PFNGLGETATTRIBLOCATIONPROC) (GLuint program, const GLchar *name);
typedef void (GL_APIENTRYP PFNGLVERTEXATTRIBPOINTERARBPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
typedef void (GL_APIENTRYP PFNGLGETACTIVEATTRIBPROC) (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);

extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
extern PFNGLVERTEXATTRIBPOINTERARBPROC glVertexAttribPointer;
extern PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib;

// ARB_fragment_shader

// ARB_vertex_array_object
#define GL_VERTEX_ARRAY_BINDING 0x85B5
#define GL_WRITE_ONLY           0x88B9
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_FRAGMENT_SHADER      0x8B30

typedef void (GL_APIENTRYP PFNGLBINDVERTEXARRAYARBPROC) (GLuint array);
typedef void (GL_APIENTRYP PFNGLDELETEVERTEXARRAYSARBPROC) (GLsizei n, const GLuint *arrays);
typedef void (GL_APIENTRYP PFNGLGENVERTEXARRAYSARBPROC) (GLsizei n, GLuint *arrays);
typedef GLboolean (GL_APIENTRYP PFNGLISVERTEXARRAYARBPROC) (GLuint array);

extern PFNGLBINDVERTEXARRAYARBPROC glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSARBPROC glDeleteVertexArrays;
extern PFNGLGENVERTEXARRAYSARBPROC glGenVertexArrays;
extern PFNGLISVERTEXARRAYARBPROC glIsVertexArray;

// GL_OES_element_index_uint
#define GL_UNSIGNED_INT 0x1405

// OpenGL 1.2
#define GL_UNSIGNED_SHORT_4_4_4_4 0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1 0x8034
#define GL_UNSIGNED_SHORT_5_6_5   0x8363

// OpenGL 1.3
#define GL_MAX_TEXTURE_UNITS 0x84E2

// OpenGL 1.5
#define GL_ARRAY_BUFFER         0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_STATIC_DRAW          0x88E4
#define GL_DYNAMIC_DRAW         0x88E8

// OpenGL 2.0
typedef void (GL_APIENTRYP PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (GL_APIENTRYP PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);

extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_IOS_GRAPHICS_OPENGL_EXTENSIONSOGL_H