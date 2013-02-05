#include "Core/Graphics/Shader.h"
#include "Core/Application/Application.h"
#include "Core/Graphics/Graphics.h"
#include "Core/Graphics/Render/RenderSystem.h"
#include "Core/Graphics/HardwareResourceProvider.h"
#include <EGEDebug.h>

#if EGE_RENDERING_OPENGL_3 || EGE_RENDERING_OPENGL_2
#include "Core/Graphics/OpenGL/ShaderOGL.h"
#endif // EGE_RENDERING_OPENGL_3 || EGE_RENDERING_OPENGL_2

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Shader)
EGE_DEFINE_DELETE_OPERATORS(Shader)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Shader::Shader(Application* app, const String& name, EGEGraphics::ShaderType type, IHardwareResourceProvider* provider) : Object(app, EGE_OBJECT_UID_SHADER), 
                                                                                                                          m_name(name), 
                                                                                                                          m_type(type),
                                                                                                                          m_provider(provider)
{
  m_p = ege_new ShaderPrivate(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Shader::~Shader()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Shader::isValid() const
{
  return (NULL != m_p) && m_p->isValid();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END