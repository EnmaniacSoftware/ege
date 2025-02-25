﻿#ifndef EGE_WIN32_GRAPHICS_OPENGL_EXTENSIONSOGLWIN32_H
#define EGE_WIN32_GRAPHICS_OPENGL_EXTENSIONSOGLWIN32_H

#include "EGE.h"
#include "EGEOpenGL.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef signed long int GLintptr;
typedef signed long int GLsizeiptr;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------  
#define GL_APIENTRYP APIENTRY*
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#define GL_NUM_EXTENSIONS 0x821D

typedef const GLubyte* (GL_APIENTRYP PFNGLGETSTRINGIPROC) (GLenum name, GLuint index);
extern PFNGLGETSTRINGIPROC glGetStringi;

// GL_ARB_multitexture
#define GL_TEXTURE0 0x84C0

typedef void (GL_APIENTRYP PFNGLCLIENTACTIVETEXTUREARBPROC) (GLenum texture);
typedef void (GL_APIENTRYP PFNGLACTIVETEXTUREARBPROC) (GLenum texture);

extern PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTexture;
extern PFNGLACTIVETEXTUREARBPROC glActiveTexture;

// GL_ARB_point_sprite
#define GL_POINT_SPRITE   0x8861
#define GL_COORD_REPLACE  0x8862

// GL_EXT_framebuffer_object
#define GL_FRAMEBUFFER_BINDING  0x8CA6
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#define GL_COLOR_ATTACHMENT0    0x8CE0
#define GL_FRAMEBUFFER          0x8D40

typedef void (GL_APIENTRYP PFNGLGENFRAMEBUFFERSPROC) (GLsizei n, GLuint *framebuffers);
typedef void (GL_APIENTRYP PFNGLDELETEFRAMEBUFFERSPROC) (GLsizei n, const GLuint *framebuffers);
typedef void (GL_APIENTRYP PFNGLBINDFRAMEBUFFERPROC) (GLenum target, GLuint framebuffer);
typedef GLenum (GL_APIENTRYP PFNGLCHECKFRAMEBUFFERSTATUSPROC) (GLenum target);
typedef void (GL_APIENTRYP PFNGLFRAMEBUFFERTEXTURE2DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GL_APIENTRYP PFNGLGENERATEMIPMAPPROC) (GLenum target);

extern PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
extern PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
extern PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;

// vertex buffer object
typedef void (GL_APIENTRYP PFNGLGENBUFFERSPROC) (GLsizei n, GLuint *buffers);
typedef void (GL_APIENTRYP PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void (GL_APIENTRYP PFNGLBUFFERDATAPROC) (GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
typedef void (GL_APIENTRYP PFNGLBUFFERSUBDATAPROC) (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
typedef void (GL_APIENTRYP PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint *buffers);
typedef GLvoid* (GL_APIENTRYP PFNGLMAPBUFFERPROC) (GLenum target, GLenum access);
typedef GLboolean (GL_APIENTRYP PFNGLUNMAPBUFFERPROC) (GLenum target);

extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLMAPBUFFERPROC glMapBuffer;
extern PFNGLUNMAPBUFFERPROC glUnmapBuffer;

// GL_ARB_point_parameters
typedef void (GL_APIENTRYP PFNGLPOINTPARAMETERFARBPROC) (GLenum pname, GLfloat param);
typedef void (GL_APIENTRYP PFNGLPOINTPARAMETERFVARBPROC) (GLenum pname, const GLfloat *params);

extern PFNGLPOINTPARAMETERFARBPROC  glPointParameterf;
extern PFNGLPOINTPARAMETERFVARBPROC glPointParameterfv;

// blend equations
typedef void (GL_APIENTRYP PFNGLBLENDEQUATIONPROC) (GLenum mode);
typedef void (GL_APIENTRYP PFNGLBLENDFUNCSEPARATEPROC) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);

extern PFNGLBLENDEQUATIONPROC glBlendEquation;
extern PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate;

// texture compression
typedef void (GL_APIENTRYP PFNGLCOMPRESSEDTEXIMAGE2DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);

extern PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;

// texture compression PVRTC
#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG			0x8C00
#define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG			0x8C01
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG			0x8C02
#define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG			0x8C03

// WGL_ARB_create_context
// WGL_ARB_create_context_profile
#define WGL_CONTEXT_MAJOR_VERSION_ARB		          0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB		          0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB		            0x2093
#define WGL_CONTEXT_FLAGS_ARB			                0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB		          0x9126
#define WGL_CONTEXT_DEBUG_BIT_ARB		              0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB	  0x0002
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB	        0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

typedef HGLRC (GL_APIENTRYP PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int* attribList); 
extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribs;

// GL_ARB_shader_objects
#define GL_COMPILE_STATUS    0x8B81
#define GL_LINK_STATUS       0x8B82
#define GL_INFO_LOG_LENGTH   0x8B84
#define GL_ACTIVE_UNIFORMS   0x8B86
#define GL_ACTIVE_ATTRIBUTES 0x8B89
#define GL_CURRENT_PROGRAM   0x8B8D

typedef char GLchar;
typedef unsigned int GLhandle;

typedef GLhandle (GL_APIENTRYP PFNGLCREATESHADERPROC) (GLenum shaderType);
typedef void (GL_APIENTRYP PFNGLDELETESHADERPROC) (GLhandle obj);
typedef void (GL_APIENTRYP PFNGLDETACHSHADERPROC) (GLhandle containerObj, GLhandle attachedObj);
typedef void (GL_APIENTRYP PFNGLGETSHADERSOURCEPROC) (GLhandle shaderObj, GLsizei count, const GLchar* *string, const GLint *length);
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

// GL_EXT_texture_compression_s3tc
#define GL_COMPRESSED_RGB_S3TC_DXT1   0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1  0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3  0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5  0x83F3

// GL_SGIS_generate_mipmap
#define GL_GENERATE_MIPMAP      0x8191
#define GL_GENERATE_MIPMAP_HINT 0x8192

// OpenGL 1.2
#define GL_UNSIGNED_SHORT_4_4_4_4 0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1 0x8034
#define GL_UNSIGNED_SHORT_5_6_5   0x8363
#define GL_CLAMP_TO_EDGE          0x812F

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

#endif // EGE_WIN32_GRAPHICS_OPENGL_EXTENSIONSOGLWIN32_H