#include "Win32/Graphics/OpenGL/ExtensionsOGLWin32.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// misc
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = NULL;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = NULL;
PFNGLGETSTRINGIPROC glGetStringi = NULL;

// multitexturing
PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTexture = NULL;
PFNGLACTIVETEXTUREARBPROC glActiveTexture = NULL;

// frame buffer object
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers = NULL;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers = NULL;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D = NULL;

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
PFNGLCREATESHADERPROC glCreateShader = NULL;
PFNGLDELETESHADERPROC glDeleteShader = NULL;
PFNGLATTACHSHADERPROC glAttachShader = NULL;
PFNGLDETACHSHADERPROC glDetachShader = NULL;
PFNGLGETSHADERSOURCEPROC glShaderSource = NULL;
PFNGLCOMPILESHADERPROC glCompileShader = NULL;
PFNGLGETSHADERIVPROC glGetShaderiv = NULL;
PFNGLCREATEPROGRAMPROC glCreateProgram = NULL;
PFNGLDELETEPROGRAMPROC glDeleteProgram = NULL;
PFNGLGETPROGRAMIVPROC glGetProgramiv = NULL;
PFNGLLINKPROGRAMARBPROC glLinkProgram = NULL;
PFNGLVALIDATEPROGRAMARBPROC glValidateProgram = NULL;
PFNGLUSEPROGRAMPROC glUseProgram = NULL;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = NULL;
PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform = NULL;
PFNGLUNIFORM1FARBPROC glUniform1f = NULL;
PFNGLUNIFORM2FARBPROC glUniform2f = NULL;
PFNGLUNIFORM3FARBPROC glUniform3f = NULL;
PFNGLUNIFORM4FARBPROC glUniform4f = NULL;
PFNGLUNIFORM1IARBPROC glUniform1i = NULL;
PFNGLUNIFORM2IARBPROC glUniform2i = NULL;
PFNGLUNIFORM3IARBPROC glUniform3i = NULL;
PFNGLUNIFORM4IARBPROC glUniform4i = NULL;
PFNGLUNIFORMMATRIX2FVARBPROC glUniformMatrix2fv = NULL;
PFNGLUNIFORMMATRIX3FVARBPROC glUniformMatrix3fv = NULL;
PFNGLUNIFORMMATRIX4FVARBPROC glUniformMatrix4fv = NULL;

// ARB_vertex_shader
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = NULL;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = NULL;
PFNGLVERTEXATTRIBPOINTERARBPROC glVertexAttribPointer = NULL;
PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib = NULL;

// ARB_vertex_array_object
PFNGLBINDVERTEXARRAYARBPROC glBindVertexArray = NULL;
PFNGLDELETEVERTEXARRAYSARBPROC glDeleteVertexArrays = NULL;
PFNGLGENVERTEXARRAYSARBPROC glGenVertexArrays = NULL;
PFNGLISVERTEXARRAYARBPROC glIsVertexArray = NULL;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
