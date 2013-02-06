#ifndef EGE_CORE_GRAPHICS_OPENGLES10_SHADER_PRIVATE_H
#define EGE_CORE_GRAPHICS_OPENGLES10_SHADER_PRIVATE_H

#include <EGE.h>
#include <EGEOpenGL.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Shader;
EGE_DECLARE_SMART_CLASS(DataBuffer, PDataBuffer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ShaderPrivate
{
  /* For accessing private data. */
  friend class RenderSystemPrivate;

  public:

    ShaderPrivate(Shader* base);
   ~ShaderPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! @see Shader::isValid. */
    bool isValid() const;
    /*! @see Shader::create. */
    EGEResult create(const PDataBuffer& buffer);

  private:

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(Shader);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_OPENGLES10_SHADER_PRIVATE_H
