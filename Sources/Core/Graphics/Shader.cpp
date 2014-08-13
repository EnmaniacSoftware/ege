#include "Core/Graphics/Shader.h"
#include "Core/Graphics/Graphics.h"
#include "Core/Graphics/Render/RenderSystem.h"
#include "Core/Graphics/HardwareResourceProvider.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Shader)
EGE_DEFINE_DELETE_OPERATORS(Shader)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Shader::Shader(const String& name, EGEGraphics::ShaderType type, IHardwareResourceProvider* provider) : Object(EGE_OBJECT_UID_SHADER)
                                                                                                      , m_name(name)
                                                                                                      , m_type(type)
                                                                                                      , m_provider(provider)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Shader::~Shader()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& Shader::name() const 
{ 
  return m_name; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEGraphics::ShaderType Shader::type() const
{
  return m_type;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END