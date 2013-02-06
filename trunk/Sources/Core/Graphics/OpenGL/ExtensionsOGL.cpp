#include "Core/Graphics/OpenGL/ExtensionsOGL.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// misc
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = NULL;

// multitexturing
PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTexture = NULL;
PFNGLACTIVETEXTUREARBPROC glActiveTexture = NULL;

// frame buffer object
PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffers = NULL;
PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffers = NULL;
PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebuffer = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatus = NULL;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2D = NULL;

// vertex buffer object
PFNGLGENBUFFERSPROC glGenBuffers = NULL;
PFNGLBINDBUFFERPROC glBindBuffer = NULL;
PFNGLBUFFERDATAPROC glBufferData = NULL;
PFNGLBUFFERSUBDATAPROC glBufferSubData = NULL;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = NULL;
PFNGLMAPBUFFERPROC glMapBuffer = NULL;
PFNGLUNMAPBUFFERPROC glUnmapBuffer = NULL;

// point sprites
PFNGLPOINTPARAMETERFARBPROC glPointParameterf = NULL;
PFNGLPOINTPARAMETERFVARBPROC glPointParameterfv = NULL;

// blend equations
PFNGLBLENDEQUATIONPROC glBlendEquation = NULL;
PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate = NULL;

// texture compression
PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D = NULL;

// WGL_ARB_create_context
// WGL_ARB_create_context_profile
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribs = NULL;

// ARB_shader_objects
PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObject = NULL;
PFNGLDELETEOBJECTARBPROC glDeleteObject = NULL;
PFNGLSHADERSOURCEARBPROC glShaderSource = NULL;
PFNGLCOMPILESHADERARBPROC glCompileShader = NULL;
PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObject = NULL;
PFNGLATTACHOBJECTARBPROC glAttachObject = NULL;
PFNGLLINKPROGRAMARBPROC glLinkProgram = NULL;
PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObject = NULL;
PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocation = NULL;
PFNGLUNIFORM1FARBPROC glUniform1f = NULL;
PFNGLUNIFORM2FARBPROC glUniform2f = NULL;
PFNGLUNIFORM3FARBPROC glUniform3f = NULL;
PFNGLUNIFORM4FARBPROC glUniform4f = NULL;
PFNGLUNIFORM1IARBPROC glUniform1i = NULL;
PFNGLUNIFORM2IARBPROC glUniform2i = NULL;
PFNGLUNIFORM3IARBPROC glUniform3i = NULL;
PFNGLUNIFORM4IARBPROC glUniform4i = NULL;
PFNGLUNIFORMMATRIX2FVARBPROC glUniformMatrix2fvARB = NULL;
PFNGLUNIFORMMATRIX3FVARBPROC glUniformMatrix3fvARB = NULL;
PFNGLUNIFORMMATRIX4FVARBPROC glUniformMatrix4fvARB = NULL;
PFNGLGETOBJECTPARAMETERFVARBPROC glGetObjectParameterfv = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameteriv = NULL;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
