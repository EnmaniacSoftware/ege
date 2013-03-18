#ifndef EGE_CORE_GRAPHICS_OPENGL_SHADEROGL_PRIVATE_H
#define EGE_CORE_GRAPHICS_OPENGL_SHADEROGL_PRIVATE_H

#include "EGE.h"
#include "EGEOpenGL.h"

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
    /*! Returns id. */
    GLhandle id() const;

  private:

    /*! Prints shader info log. */
    void printInfoLog();

  private:

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(Shader);

    /*! Shader id. */
    GLhandle m_id;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline GLhandle ShaderPrivate::id() const
{
  return m_id;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_OPENGL_SHADEROGL_PRIVATE_H
