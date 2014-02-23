#include "Core/Graphics/Shader.h"
#include "Core/Application/Application.h"
#include "Core/Graphics/Graphics.h"
#include "Core/Graphics/Render/RenderSystem.h"
#include "Core/Graphics/HardwareResourceProvider.h"
#include "EGEDebug.h"

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
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Shader::~Shader()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END