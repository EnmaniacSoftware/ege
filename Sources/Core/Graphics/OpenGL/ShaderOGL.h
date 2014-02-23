#ifndef EGE_CORE_GRAPHICS_OPENGL_SHADEROGL_PRIVATE_H
#define EGE_CORE_GRAPHICS_OPENGL_SHADEROGL_PRIVATE_H

#include "EGE.h"
#include "EGEOpenGL.h"
#include "EGEShader.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Shader;
EGE_DECLARE_SMART_CLASS(DataBuffer, PDataBuffer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ShaderOGL : public Shader
{
  public:

    ShaderOGL(Application* app, const String& name, EGEGraphics::ShaderType type, IHardwareResourceProvider* provider);
    virtual ~ShaderOGL();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! @see Shader::isValid. */
    bool isValid() const override;
    /*! @see Shader::create. */
    EGEResult create(const PDataBuffer& buffer);
    /*! Returns id. */
    GLhandle id() const;

  private:

    /*! Prints shader info log. */
    void printInfoLog();

  private:

    /*! Shader id. */
    GLhandle m_id;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_OPENGL_SHADEROGL_PRIVATE_H
