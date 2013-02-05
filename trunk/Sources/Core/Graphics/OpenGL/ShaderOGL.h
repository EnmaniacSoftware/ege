#ifndef EGE_CORE_GRAPHICS_OPENGL_SHADEROGL_PRIVATE_H
#define EGE_CORE_GRAPHICS_OPENGL_SHADEROGL_PRIVATE_H

#include <EGE.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Shader;
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

    /*! Returns TRUE if object is valid. */
    bool isValid() const;

  private:

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(Shader);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_OPENGL_SHADEROGL_PRIVATE_H
