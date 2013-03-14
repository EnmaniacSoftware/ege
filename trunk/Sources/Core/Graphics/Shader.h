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
  /* For accessing private data. */
  friend class RenderSystemPrivate;
  friend class ProgramPrivate;

  public:

    virtual ~Shader();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Returns TRUE if object is valid. */
    bool isValid() const;
    /*! Returns name. */
    const String& name() const;
    /*! Returns shader type. */
    EGEGraphics::ShaderType type() const;
    /*! Creates shader from given buffer. 
     *  @note Calling thread must be able to issue underlying 3D API commands.
     */
    EGEResult create(const PDataBuffer& buffer);

  protected:

    /*! Constructing only via RenderSystem. */
    Shader(Application* app, const String& name, EGEGraphics::ShaderType type, IHardwareResourceProvider* provider);

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(Shader);

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
