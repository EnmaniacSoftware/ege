#ifndef EGE_CORE_GRAPHICS_PROGRAM_H
#define EGE_CORE_GRAPHICS_PROGRAM_H

/** Base object representing programmable (shader) program.
*/

#include "EGE.h"
#include "EGEString.h"
#include "EGEShader.h"
#include "EGEList.h"
#include "EGEComponent.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Shader, PShader)
EGE_DECLARE_SMART_CLASS(Program, PProgram)
class IHardwareResourceProvider;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Program : public Component
{
  public:

    virtual ~Program();

  public:

    /*! Returns TRUE if object is valid. */
    virtual bool isValid() const = 0;
    /*! Attaches given shader. 
     *  @note Calling thread must be able to issue underlying 3D API commands.
     */
    virtual bool attach(const PShader& shader);
    /*! Detaches given shader. 
     *  @note Calling thread must be able to issue underlying 3D API commands.
     */
    virtual bool detach(const PShader& shader);
    /*! Detaches all shaders. 
     *  @note Calling thread must be able to issue underlying 3D API commands.
     */
    bool detachAll();
    /*! Binds program to GPU. */
    virtual void bind() = 0;
    /*! Unbinds program from GPU. */
    virtual void unbind() = 0;
    
  protected:

    /*! Constructing only via RenderSystem. */
    Program(const String& name, IHardwareResourceProvider* provider);

  private:

    /*! Resource provider used to create program. */
    IHardwareResourceProvider* m_provider;
    /*! List of attached shaders. */
    List<PShader> m_attachedShaders;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_PROGRAM_H
