#ifndef EGE_CORE_EXTENSIONSOGL_H
#define EGE_CORE_EXTENSIONSOGL_H

#include "EGE.h"
#include "EGEOpenGL.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef signed long int GLintptr;
typedef signed long int GLsizeiptr;

#define GL_NUM_EXTENSIONS 0x821D

typedef const GLubyte* (APIENTRY* PFNGLGETSTRINGIPROC) (GLenum name, GLuint index);
extern PFNGLGETSTRINGIPROC glGetStringi;

// GL_ARB_multitexture
#define GL_TEXTURE0 0x84C0

typedef void (APIENTRY* PFNGLCLIENTACTIVETEXTUREARBPROC) (GLenum texture);
typedef void (APIENTRY* PFNGLACTIVETEXTUREARBPROC) (GLenum texture);

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

typedef void (APIENTRY* PFNGLGENFRAMEBUFFERSEXTPROC) (GLsizei n, GLuint *framebuffers);
typedef void (APIENTRY* PFNGLDELETEFRAMEBUFFERSEXTPROC) (GLsizei n, const GLuint *framebuffers);
typedef void (APIENTRY* PFNGLBINDFRAMEBUFFEREXTPROC) (GLenum target, GLuint framebuffer);
typedef GLenum (APIENTRY* PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC) (GLenum target);
typedef void (APIENTRY* PFNGLFRAMEBUFFERTEXTURE2DEXTPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);

extern PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffers;
extern PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffers;
extern PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebuffer;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatus;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2D;

// vertex buffer object
typedef void (APIENTRY* PFNGLGENBUFFERSPROC) (GLsizei n, GLuint *buffers);
typedef void (APIENTRY* PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRY* PFNGLBUFFERDATAPROC) (GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
typedef void (APIENTRY* PFNGLBUFFERSUBDATAPROC) (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
typedef void (APIENTRY* PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint *buffers);
typedef GLvoid* (APIENTRY* PFNGLMAPBUFFERPROC) (GLenum target, GLenum access);
typedef GLboolean (APIENTRY* PFNGLUNMAPBUFFERPROC) (GLenum target);

extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLMAPBUFFERPROC glMapBuffer;
extern PFNGLUNMAPBUFFERPROC glUnmapBuffer;

// GL_ARB_point_parameters
typedef void (APIENTRY* PFNGLPOINTPARAMETERFARBPROC) (GLenum pname, GLfloat param);
typedef void (APIENTRY* PFNGLPOINTPARAMETERFVARBPROC) (GLenum pname, const GLfloat *params);

extern PFNGLPOINTPARAMETERFARBPROC  glPointParameterf;
extern PFNGLPOINTPARAMETERFVARBPROC glPointParameterfv;

// blend equations
typedef void (APIENTRY* PFNGLBLENDEQUATIONPROC) (GLenum mode);
typedef void (APIENTRY* PFNGLBLENDFUNCSEPARATEPROC) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);

extern PFNGLBLENDEQUATIONPROC glBlendEquation;
extern PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate;

// texture compression
typedef void (APIENTRY* PFNGLCOMPRESSEDTEXIMAGE2DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);

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

typedef HGLRC (APIENTRY* PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int* attribList); 
extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribs;

// GL_ARB_shader_objects
#define GL_OBJECT_COMPILE_STATUS  0x8B81
#define GL_OBJECT_LINK_STATUS     0x8B82
#define GL_OBJECT_INFO_LOG_LENGTH 0x8B84
#define GL_OBJECT_ACTIVE_UNIFORMS 0x8B86

typedef char GLchar;
typedef unsigned int GLhandle;

typedef GLhandle (APIENTRY* PFNGLCREATESHADEROBJECTARBPROC) (GLenum shaderType);
typedef void (APIENTRY* PFNGLDELETEOBJECTARBPROC) (GLhandle obj);
typedef void (APIENTRY* PFNGLDETACHOBJECTARBPROC) (GLhandle containerObj, GLhandle attachedObj);
typedef void (APIENTRY* PFNGLSHADERSOURCEARBPROC) (GLhandle shaderObj, GLsizei count, const GLchar* *string, const GLint *length);
typedef void (APIENTRY* PFNGLCOMPILESHADERARBPROC) (GLhandle shaderObj);
typedef GLhandle (APIENTRY* PFNGLCREATEPROGRAMOBJECTARBPROC) (void);
typedef void (APIENTRY* PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint *params);
typedef void (APIENTRY* PFNGLATTACHOBJECTARBPROC) (GLhandle containerObj, GLhandle obj);
typedef void (APIENTRY* PFNGLLINKPROGRAMARBPROC) (GLhandle programObj);
typedef void (APIENTRY* PFNGLVALIDATEPROGRAMARBPROC) (GLhandle programObj);
typedef void (APIENTRY* PFNGLUSEPROGRAMOBJECTARBPROC) (GLhandle programObj);
typedef GLint (APIENTRY* PFNGLGETUNIFORMLOCATIONARBPROC) (GLhandle programObj, const GLchar*name);
typedef void (APIENTRY* PFNGLGETACTIVEUNIFORMPROC) (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
typedef void (APIENTRY* PFNGLUNIFORM1FARBPROC) (GLint location, GLfloat v0);
typedef void (APIENTRY* PFNGLUNIFORM2FARBPROC) (GLint location, GLfloat v0, GLfloat v1);
typedef void (APIENTRY* PFNGLUNIFORM3FARBPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (APIENTRY* PFNGLUNIFORM4FARBPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (APIENTRY* PFNGLUNIFORM1IARBPROC) (GLint location, GLint v0);
typedef void (APIENTRY* PFNGLUNIFORM2IARBPROC) (GLint location, GLint v0, GLint v1);
typedef void (APIENTRY* PFNGLUNIFORM3IARBPROC) (GLint location, GLint v0, GLint v1, GLint v2);
typedef void (APIENTRY* PFNGLUNIFORM4IARBPROC) (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (APIENTRY* PFNGLUNIFORMMATRIX2FVARBPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (APIENTRY* PFNGLUNIFORMMATRIX3FVARBPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (APIENTRY* PFNGLUNIFORMMATRIX4FVARBPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (APIENTRY* PFNGLGETOBJECTPARAMETERFVARBPROC) (GLhandle obj, GLenum pname, GLfloat *params);
typedef void (APIENTRY* PFNGLGETOBJECTPARAMETERIVARBPROC) (GLhandle obj, GLenum pname, GLint *params);

extern PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObject;
extern PFNGLDELETEOBJECTARBPROC glDeleteObject;
extern PFNGLDETACHOBJECTARBPROC glDetachObject;
extern PFNGLSHADERSOURCEARBPROC glShaderSource;
extern PFNGLCOMPILESHADERARBPROC glCompileShader;
extern PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObject;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLATTACHOBJECTARBPROC glAttachObject;
extern PFNGLLINKPROGRAMARBPROC glLinkProgram;
extern PFNGLVALIDATEPROGRAMARBPROC glValidateProgram;
extern PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObject;
extern PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocation;
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
extern PFNGLGETOBJECTPARAMETERFVARBPROC glGetObjectParameterfv;
extern PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameteriv;

// ARB_vertex_shader
typedef void (APIENTRY* PFNGLDISABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (APIENTRY* PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef GLint (APIENTRY* PFNGLGETATTRIBLOCATIONPROC) (GLuint program, const GLchar *name);
typedef void (APIENTRY* PFNGLVERTEXATTRIBPOINTERARBPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);

extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
extern PFNGLVERTEXATTRIBPOINTERARBPROC glVertexAttribPointer;

// ARB_fragment_shader

// ARB_vertex_array_object
#define GL_VERTEX_ARRAY_BINDING     0x85B5
#define GL_WRITE_ONLY               0x88B9
#define GL_ELEMENT_ARRAY_BUFFER_ARB 0x8893

typedef void (APIENTRY* PFNGLBINDVERTEXARRAYARBPROC) (GLuint array);
typedef void (APIENTRY* PFNGLDELETEVERTEXARRAYSARBPROC) (GLsizei n, const GLuint *arrays);
typedef void (APIENTRY* PFNGLGENVERTEXARRAYSARBPROC) (GLsizei n, GLuint *arrays);
typedef GLboolean (APIENTRY* PFNGLISVERTEXARRAYARBPROC) (GLuint array);

extern PFNGLBINDVERTEXARRAYARBPROC glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSARBPROC glDeleteVertexArrays;
extern PFNGLGENVERTEXARRAYSARBPROC glGenVertexArrays;
extern PFNGLISVERTEXARRAYARBPROC glIsVertexArray;

// GL_EXT_texture_compression_s3tc
#define GL_COMPRESSED_RGB_S3TC_DXT1   0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1  0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3  0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5  0x83F3

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
typedef void (APIENTRY* PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (APIENTRY* PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);

extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_CORE_EXTENSIONSOGL_H