#include "EGEDataBuffer.h"
#include "EGEOpenGL.h"
#include "Core/Graphics/OpenGL/ShaderOGL.h"
#include "EGEDevice.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Maps shader type to OpenGL compilant one. */
static GLenum MapShaderType(EGEGraphics::ShaderType type)
{
  GLenum result = 0;

  switch (type)
  {
    case EGEGraphics::FRAGMENT_SHADER:  result = GL_FRAGMENT_SHADER; break;
    case EGEGraphics::VERTEX_SHADER:    result = GL_VERTEX_SHADER; break;
    
    default:
    
      EGE_ASSERT_X(false, "Unsupported shader type");
      break;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ShaderOGL)
EGE_DEFINE_DELETE_OPERATORS(ShaderOGL)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ShaderOGL::ShaderOGL(const String& name, EGEGraphics::ShaderType type, IHardwareResourceProvider* provider) 
: Shader(name, type, provider)
, m_id(0)
{
  m_id = glCreateShader(MapShaderType(type));
  OGL_CHECK();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ShaderOGL::~ShaderOGL()
{
  if (0 != m_id)
  {
    glDeleteShader(m_id);
    OGL_CHECK();
    m_id = 0;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ShaderOGL::isValid() const
{
  return (0 != m_id);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ShaderOGL::create(const PDataBuffer& buffer)
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
  glGetShaderiv(m_id, GL_COMPILE_STATUS, &compileResult);
  if (GL_TRUE != compileResult)
  {
    // error!
    printInfoLog();
    return EGE_ERROR;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ShaderOGL::printInfoLog()
{
  int logLength = 0;
  int charsWritten  = 0;
  char* log;

  // get log length
  glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &logLength);
  if (0 < logLength)
  {
    // allocate space for the log
	  log = reinterpret_cast<char*>(EGE_MALLOC(logLength));
	  if ((NULL != log) && (NULL != glGetShaderInfoLog))
    {
      // retrieve log
      glGetShaderInfoLog(m_id, logLength, &charsWritten, log);
      egeCritical(KOpenGLDebugName) << log;
    }

    // clean up
  	EGE_FREE(log);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
GLhandle ShaderOGL::id() const
{
  return m_id;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END