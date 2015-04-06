#ifndef EGE_CORE_GRAPHICS_OPENGL_PROGRAMOGL_PRIVATE_H
#define EGE_CORE_GRAPHICS_OPENGL_PROGRAMOGL_PRIVATE_H

#include "EGE.h"
#include "EGEOpenGL.h"
#include "EGEMap.h"
#include "EGEShader.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_PREDECLARE_SMART_CLASS(Shader, PShader)
class Program;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ProgramOGL : public Program
{
  public:

    ProgramOGL(const String& name, IHardwareResourceProvider* provider);
    virtual ~ProgramOGL();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! @see Program::isValid. */
    bool isValid() const override;
    /*! @see Program::attach. */
    bool attach(const PShader& shader) override;
    /*! @see Program::detach. */
    bool detach(const PShader& shader) override;
     /*! Links the program. 
      *  @note Calling thread must be able to issue underlying 3D API commands.
      */
    bool link();
    /*! @see Program::bind. */
    void bind() override;
    /*! @see Program::unbind. */
    void unbind() override;

    /*! Returns location infex of a given uniform. 
     *  @param  name  Name of the uniform.
     *  @return Location index (0-based) of a given uniform within the program. Negative, if could not be found.
     */
    GLint uniformLocation(const String& name) const;
    /*! Returns location infex of a given attribute. 
     *  @param  name  Name of the attribute.
     *  @return Location index (0-based) of a given attribute within the program. Negative, if could not be found.
     */
    GLint attributeLocation(const String& name) const;

  private:

    /*! Prints program info log. */
    void printInfoLog();
    /*! Builds uniforms list. */
    bool buildUniformsList();
    /*! Builds attributes list. */
    bool buildAttributesList();

  private:

    /*! Program id. */
    GLhandle m_id;
    /*! Flag indicating if program has been linked. */
    bool m_linked;
    /*! Map of uniforms and locations within program. */
    Map<String, GLint> m_uniforms;
    /*! Map of attributes and locations within program. */
    Map<String, GLint> m_attributes;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_OPENGL_PROGRAMOGL_PRIVATE_H
