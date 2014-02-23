#ifndef EGE_CORE_GRAPHICS_SHADER_H
#define EGE_CORE_GRAPHICS_SHADER_H

/** Base object representing programmable shader.
*/

#include "EGE.h"
#include "EGEString.h"
#include "EGEShader.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Shader, PShader)
EGE_DECLARE_SMART_CLASS(DataBuffer, PDataBuffer)
class IHardwareResourceProvider;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Shader : public Object
{
  public:

    virtual ~Shader();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Returns TRUE if object is valid. */
    virtual bool isValid() const = 0;
    /*! Returns name. */
    const String& name() const;
    /*! Returns shader type. */
    EGEGraphics::ShaderType type() const;

  protected:

    /*! Constructing only via RenderSystem. */
    Shader(Application* app, const String& name, EGEGraphics::ShaderType type, IHardwareResourceProvider* provider);

  private:

    /*! Name. */
    String m_name; 
    /*! Type. */
    EGEGraphics::ShaderType m_type;
    /*! Resource provider used to create shader. */
    IHardwareResourceProvider* m_provider;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline const String& Shader::name() const 
{ 
  return m_name; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline EGEGraphics::ShaderType Shader::type() const
{
  return m_type;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_SHADER_H
