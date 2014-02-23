#include "Core/Graphics/Program.h"
#include "Core/Application/Application.h"
#include "Core/Graphics/Graphics.h"
#include "Core/Graphics/Render/RenderSystem.h"
#include "Core/Graphics/HardwareResourceProvider.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Program::Program(Application* app, const String& name, IHardwareResourceProvider* provider) : Component(app, EGE_OBJECT_UID_PROGRAM, name), 
                                                                                              m_provider(provider)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Program::~Program()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Program::attach(const PShader& shader)
{
  // add to pool
  m_attachedShaders << shader;
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Program::detach(const PShader& shader)
{
  // remove from pool
  m_attachedShaders.remove(shader);
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Program::detachAll()
{
  // make local copy
  // NOTE: this is to make sure iterators are not invalidated while looping as shaders get removed in detach method
  List<PShader> copy(m_attachedShaders);

  bool result = true;
  for (List<PShader>::const_iterator it = copy.begin(); it != copy.end(); ++it)
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

EGE_NAMESPACE_END