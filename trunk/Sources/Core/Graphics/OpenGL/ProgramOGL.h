#ifndef EGE_CORE_GRAPHICS_OPENGL_PROGRAMOGL_PRIVATE_H
#define EGE_CORE_GRAPHICS_OPENGL_PROGRAMOGL_PRIVATE_H

#include <EGE.h>
#include <EGEOpenGL.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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

    /*! @see Shader::isValid. */
    bool isValid() const;

  private:

    /*! Prints shader info log. */
    void printInfoLog();

  private:

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(Program);

    /*! Shader handle. */
    GLhandle m_handle;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_OPENGL_PROGRAMOGL_PRIVATE_H
