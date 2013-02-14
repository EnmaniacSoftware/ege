#include <EGEOpenGL.h>
#include "Core/Graphics/OpenGL/ProgramOGL.h"
#include "Core/Graphics/OpenGL/ShaderOGL.h"
#include <EGEShader.h>
#include <EGEDevice.h>
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ProgramPrivate)
EGE_DEFINE_DELETE_OPERATORS(ProgramPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ProgramPrivate::ProgramPrivate(Program* base) : m_d(base),
                                                m_id(0),
                                                m_linked(false)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ProgramPrivate::~ProgramPrivate()
{
  // NOTE: at this point object should be deallocated
  EGE_ASSERT(0 == m_id);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ProgramPrivate::isValid() const
{
  return (0 != m_id) && m_linked;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ProgramPrivate::attach(const PShader& shader)
{
  GLenum result = GL_NO_ERROR;

  glAttachObject(m_id, shader->p_func()->id());
  OGL_CHECK_RESULT(result);

  return (GL_NO_ERROR == result);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ProgramPrivate::detach(const PShader& shader)
{
  GLenum result = GL_NO_ERROR;

  glDetachObject(m_id, shader->p_func()->id());
  OGL_CHECK_RESULT(result);

  return (GL_NO_ERROR == result);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ProgramPrivate::link()
{
  // link
  glLinkProgram(m_id);

  // validate
  //glValidateProgram(m_id);

  int linkResult;
  glGetObjectParameteriv(m_id, GL_OBJECT_LINK_STATUS_ARB, &linkResult);
  if (GL_TRUE != linkResult)
  {
    // error!
    printInfoLog();
    return false;
  }

  // bind it so uniforms can be quereied
  glUseProgramObject(m_id);
  OGL_CHECK();

  // retrieve uniform data
  if ( ! buildUniformsList())
  {
    // error!
    return false;
  }

  // set flag
  m_linked = true;

  // unbind
  glUseProgramObject(0);
  OGL_CHECK();

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ProgramPrivate::buildUniformsList()
{
  // retrive number of uniforms
  GLint total = 0;
  glGetObjectParameteriv(m_id, GL_OBJECT_ACTIVE_UNIFORMS_ARB, &total); 
  for (GLint i = 0; i < total; ++i)  
  {
    GLsizei nameLength = -1;
    GLint num = -1;

    GLenum type = GL_ZERO;
    
    GLchar name[128];

    // get uniform data
    glGetActiveUniform(m_id, i, sizeof(name) - 1, &nameLength, &num, &type, name);

    // zero terminate
    name[nameLength] = 0;

    // retrieve location
    GLuint location = 0;
    location = glGetUniformLocation(m_id, name);

    // add to pool
    m_uniforms.insert(name, location);
  }

  // TAGE - for attribs
  // glGetAttribLocation

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ProgramPrivate::printInfoLog()
{
  int logLength = 0;
  int charsWritten  = 0;
  char* log;

  // get log length
  glGetObjectParameteriv(m_id, GL_OBJECT_INFO_LOG_LENGTH_ARB, &logLength);
  if (0 < logLength)
  {
    // allocate space for the log
	  log = reinterpret_cast<char*>(EGE_MALLOC(logLength));
	  if ((NULL != log) && (NULL != glGetProgramInfoLog))
    {
      // retrieve log
      glGetProgramInfoLog(m_id, logLength, &charsWritten, log);
      egeCritical() << log;
    }

    // clean up
  	EGE_FREE(log);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END