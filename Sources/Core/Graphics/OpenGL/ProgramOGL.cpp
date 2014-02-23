#include "EGEOpenGL.h"
#include "Core/Graphics/OpenGL/ProgramOGL.h"
#include "Core/Graphics/OpenGL/ShaderOGL.h"
#include "EGEShader.h"
#include "EGEDevice.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ProgramOGL)
EGE_DEFINE_DELETE_OPERATORS(ProgramOGL)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ProgramOGL::ProgramOGL(Application* app, const String& name, IHardwareResourceProvider* provider) : Program(app, name, provider),
                                                                                                    m_id(0),
                                                                                                    m_linked(false)
{
  egeDebug(KOpenGLDebugName) << glCreateProgram;
  m_id = glCreateProgram();
  OGL_CHECK();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ProgramOGL::~ProgramOGL()
{
  if (0 != m_id)
  {
    // detach shaders
    detachAll();

    // delete program
    glDeleteProgram(m_id);
    OGL_CHECK();
    m_id = 0;
  }

  m_linked = false;
  m_uniforms.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ProgramOGL::isValid() const
{
  return (0 != m_id);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ProgramOGL::attach(const PShader& shader)
{
  ShaderOGL* shaderOGL = ege_cast<ShaderOGL*>(shader);

  GLenum result = GL_NO_ERROR;

  glAttachShader(m_id, shaderOGL->id());
  OGL_CHECK_RESULT(result);

  if (GL_NO_ERROR == result)
  {
    // call base class
    result = Program::attach(shader) ? GL_NO_ERROR : GL_OUT_OF_MEMORY;
  }

  return (GL_NO_ERROR == result);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ProgramOGL::detach(const PShader& shader)
{
  ShaderOGL* shaderOGL = ege_cast<ShaderOGL*>(shader);

  GLenum result = GL_NO_ERROR;

  glDetachShader(m_id, shaderOGL->id());
  OGL_CHECK_RESULT(result);

  if (GL_NO_ERROR == result)
  {
    // call base class
    result = Program::detach(shader) ? GL_NO_ERROR : GL_OUT_OF_MEMORY;
  }

  return (GL_NO_ERROR == result);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ProgramOGL::link()
{
  // link
  glLinkProgram(m_id);

  // validate
  glValidateProgram(m_id);

  int linkResult;
  glGetProgramiv(m_id, GL_LINK_STATUS, &linkResult);
  if (GL_TRUE != linkResult)
  {
    // error!
    printInfoLog();
    return false;
  }

  // bind it so uniforms can be quereied
  bind();

  // retrieve uniform and attributes data
  if ( ! buildUniformsList() || ! buildAttributesList())
  {
    // error!
    return false;
  }

  // set flag
  m_linked = true;

  // unbind
  unbind();

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ProgramOGL::buildUniformsList()
{
  // retrive number of uniforms
  GLint total = 0;
  glGetProgramiv(m_id, GL_ACTIVE_UNIFORMS, &total); 
  OGL_CHECK();

  for (GLint i = 0; i < total; ++i)  
  {
    GLsizei nameLength = -1;
    GLint num = -1;

    GLenum type = GL_ZERO;
    
    GLchar name[128];

    // get uniform data
    glGetActiveUniform(m_id, i, sizeof(name) - 1, &nameLength, &num, &type, name);
    OGL_CHECK();

    // retrieve location
    GLint location = 0;
    location = glGetUniformLocation(m_id, name);
    OGL_CHECK();

    // add to pool
    m_uniforms.insert(name, location);
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ProgramOGL::buildAttributesList()
{
  // retrive number of uniforms
  GLint total = 0;
  glGetProgramiv(m_id, GL_ACTIVE_ATTRIBUTES, &total); 
  OGL_CHECK();

  for (GLint i = 0; i < total; ++i)  
  {
    GLsizei nameLength = -1;
    GLint num = -1;

    GLenum type = GL_ZERO;
    
    GLchar name[128];

    // get uniform data
    glGetActiveAttrib(m_id, i, sizeof(name) - 1, &nameLength, &num, &type, name);
    OGL_CHECK();

    // retrieve location
    GLuint location = 0;
    location = glGetAttribLocation(m_id, name);
    OGL_CHECK();

    // add to pool
    m_attributes.insert(name, location);
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ProgramOGL::printInfoLog()
{
  int logLength = 0;
  int charsWritten  = 0;
  char* log;

  // get log length
  glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &logLength);
  if (0 < logLength)
  {
    // allocate space for the log
	  log = reinterpret_cast<char*>(EGE_MALLOC(logLength));
	  if ((NULL != log) && (NULL != glGetProgramInfoLog))
    {
      // retrieve log
      glGetProgramInfoLog(m_id, logLength, &charsWritten, log);
      egeCritical(KOpenGLDebugName) << log;
    }

    // clean up
  	EGE_FREE(log);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ProgramOGL::bind()
{
  glUseProgram(m_id);
  OGL_CHECK();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ProgramOGL::unbind()
{
  // unbind
  glUseProgram(0);
  OGL_CHECK();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
GLuint ProgramOGL::uniformLocation(const String& name) const
{
  return m_uniforms.value(name, -1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
GLuint ProgramOGL::attributeLocation(const String& name) const
{
  return m_attributes.value(name, -1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END