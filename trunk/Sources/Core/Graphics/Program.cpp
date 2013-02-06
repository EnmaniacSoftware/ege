#include "Core/Graphics/Program.h"
#include "Core/Application/Application.h"
#include "Core/Graphics/Graphics.h"
#include "Core/Graphics/Render/RenderSystem.h"
#include "Core/Graphics/HardwareResourceProvider.h"
#include <EGEDebug.h>

#if EGE_RENDERING_OPENGL_3 || EGE_RENDERING_OPENGL_2 || EGE_RENDERING_OPENGLES_1
#include "Core/Graphics/OpenGL/ProgramOGL.h"
#elif EGE_RENDERING_OPENGLES_1
#include "Core/Graphics/OpenGL/ES 1.0/ProgramOGLES1_p.h"
#endif // EGE_RENDERING_OPENGL_3 || EGE_RENDERING_OPENGL_2

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

EGE_NAMESPACE_END