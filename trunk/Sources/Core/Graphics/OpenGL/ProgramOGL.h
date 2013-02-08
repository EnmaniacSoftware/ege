#ifndef EGE_CORE_GRAPHICS_OPENGL_PROGRAMOGL_PRIVATE_H
#define EGE_CORE_GRAPHICS_OPENGL_PROGRAMOGL_PRIVATE_H

#include <EGE.h>
#include <EGEOpenGL.h>
#include <EGEMap.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Shader, PShader)
class Program;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ProgramPrivate
{
  /* For accessing private data. */
  friend class RenderSystemPrivate;

  public:

    ProgramPrivate(Program* base);
   ~ProgramPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! @see Program::isValid. */
    bool isValid() const;
    /*! @see Program::attach. */
    bool attach(const PShader& shader);
    /*! @see Program::detach. */
    bool detach(const PShader& shader);
    /*! @see Program::link. */
    bool link();

  private:

    /*! Prints program info log. */
    void printInfoLog();
    /*! Builds uniforms list. */
    bool buildUniformsList();

  private:

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(Program);

    /*! Program id. */
    GLhandle m_id;
    /*! Flag indicating if program has been linked. */
    bool m_linked;
    /*! Map of uniforms and locations within program. */
    Map<String, GLuint> m_uniforms;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_OPENGL_PROGRAMOGL_PRIVATE_H
