#ifndef EGE_CORE_GRAPHICS_OPENGLES10_PROGRAM_PRIVATE_H
#define EGE_CORE_GRAPHICS_OPENGLES10_PROGRAM_PRIVATE_H

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

    /*! @see Program::isValid. */
    bool isValid() const;
    /*! @see Program::attach. */
    bool attach(const PShader& shader);
    /*! @see Program::detach. */
    bool detach(const PShader& shader);
    /*! @see Program::link. */
    bool link();

  private:

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(Program);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_OPENGLES10_PROGRAM_PRIVATE_H
