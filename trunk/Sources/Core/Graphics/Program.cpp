#include "Core/Graphics/Program.h"
#include "Core/Application/Application.h"
#include "Core/Graphics/Graphics.h"
#include "Core/Graphics/Render/RenderSystem.h"
#include "Core/Graphics/HardwareResourceProvider.h"
#include "EGEDebug.h"

//#if EGE_RENDERING_OPENGL_3 || EGE_RENDERING_OPENGL_FIXED
#include "Core/Graphics/OpenGL/ProgramOGL.h"
//#elif EGE_RENDERING_OPENGLES_1
//#include "Core/Graphics/OpenGL/ES 1.0/ProgramOGLES1_p.h"
//#endif // EGE_RENDERING_OPENGL_3 || EGE_RENDERING_OPENGL_FIXED

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Program)
EGE_DEFINE_DELETE_OPERATORS(Program)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Program::Program(Application* app, const String& name, IHardwareResourceProvider* provider) : Object(app, EGE_OBJECT_UID_PROGRAM), 
                                                                                              m_name(name),
                                                                                              m_provider(provider)
{
  m_p = ege_new ProgramPrivate(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Program::~Program()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Program::isValid() const
{
  return (NULL != m_p) && m_p->isValid();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Program::attach(const PShader& shader)
{
  bool result = p_func()->attach(shader);
  if (result)
  {
    // add to pool
    m_attachedShaders << shader;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Program::detach(const PShader& shader)
{
  bool result = p_func()->detach(shader);
  if (result)
  {
    // remove from pool
    m_attachedShaders.remove(shader);
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Program::detachAll()
{
  bool result = true;
  for (List<PShader>::const_iterator it = m_attachedShaders.begin(); it != m_attachedShaders.end(); ++it)
  {
    const PShader& shader = *it;

    if ( ! detach(shader))
    {
      // error!
      result = false;
    }
  }
  m_attachedShaders.clear();

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Program::link()
{
  return p_func()->link();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END