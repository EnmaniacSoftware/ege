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
                                                m_handle(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ProgramPrivate::~ProgramPrivate()
{
  // NOTE: at this point object should be deallocated
  EGE_ASSERT(0 == m_handle);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ProgramPrivate::isValid() const
{
  return (0 != m_handle);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ProgramPrivate::printInfoLog()
{
  //int logLength = 0;
  //int charsWritten  = 0;
  //char* log;

  //// get log length
  //glGetObjectParameteriv(m_handle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &logLength);
  //if (0 < logLength)
  //{
  //  // allocate space for the log
	 // log = reinterpret_cast<char*>(EGE_MALLOC(logLength));
	 // if (NULL != log)
  //  {
  //    // retrieve log
  //    glGetShaderInfoLog(m_handle, logLength, &charsWritten, log);
  //    egeCritical() << log;

  //    // clean up
  //	  EGE_FREE(log);
  //  }
  //}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END