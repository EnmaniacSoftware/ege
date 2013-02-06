#ifndef EGE_CORE_GRAPHICS_PROGRAM_H
#define EGE_CORE_GRAPHICS_PROGRAM_H

/** Base object representing programmable (shader) program.
*/

#include <EGE.h>
#include <EGEString.h>
#include <EGEShader.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Shader, PShader)
EGE_DECLARE_SMART_CLASS(Program, PProgram)
class IHardwareResourceProvider;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Program : public Object
{
  /* For accessing private data. */
  friend class RenderSystemPrivate;
  
  public:

    virtual ~Program();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Returns TRUE if object is valid. */
    bool isValid() const;
    /*! Returns name. */
    const String& name() const;

  protected:

    /*! Constructing only via RenderSystem. */
    Program(Application* app, const String& name, IHardwareResourceProvider* provider);

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(Program);

    /*! Name. */
    String m_name; 
    /*! Resource provider used to create program. */
    IHardwareResourceProvider* m_provider;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline const String& Program::name() const 
{ 
  return m_name; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_PROGRAM_H
