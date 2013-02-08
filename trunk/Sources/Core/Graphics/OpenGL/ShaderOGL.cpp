#include <EGEDataBuffer.h>
#include <EGEOpenGL.h>
#include "Core/Graphics/OpenGL/ShaderOGL.h"
#include <EGEShader.h>
#include <EGEDevice.h>
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ShaderPrivate)
EGE_DEFINE_DELETE_OPERATORS(ShaderPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ShaderPrivate::ShaderPrivate(Shader* base) : m_d(base),
                                             m_id(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ShaderPrivate::~ShaderPrivate()
{
  // NOTE: at this point object should be deallocated
  EGE_ASSERT(0 == m_id);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ShaderPrivate::isValid() const
{
  return (0 != m_id);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ShaderPrivate::create(const PDataBuffer& buffer)
{
  GLint length = static_cast<GLint>(buffer->size() - buffer->readOffset());
  const GLchar* source = reinterpret_cast<GLchar*>(buffer->data(buffer->readOffset()));

  // set source
  glShaderSource(m_id, 1, &source, &length);
  
  GLenum result = GL_NO_ERROR;
  OGL_CHECK_RESULT(result);

  if (GL_NO_ERROR != result)
  {
    // error!
    return EGE_ERROR;
  }

  // compile
  glCompileShader(m_id);

  int compileResult;
  glGetObjectParameteriv(m_id, GL_OBJECT_COMPILE_STATUS_ARB, &compileResult);
  if (GL_TRUE != compileResult)
  {
    // error!
    printInfoLog();
    return EGE_ERROR;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ShaderPrivate::printInfoLog()
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
	  if ((NULL != log) && (NULL != glGetShaderInfoLog))
    {
      // retrieve log
      glGetShaderInfoLog(m_id, logLength, &charsWritten, log);
      egeCritical() << log;
    }

    // clean up
  	EGE_FREE(log);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END