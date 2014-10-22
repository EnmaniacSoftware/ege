#ifndef EGE_CORE_RESOURCEPROGRAM_H
#define EGE_CORE_RESOURCEPROGRAM_H

/** Program resource definition class. This object (resource) contains definition of shader program object.
 *  Program resource can define set of shaders to be used together. Upon loading it contains program object to be used in rendering.
 */

#include "EGE.h"
#include "EGEString.h"
#include "EGEXml.h"
#include "EGEMap.h"
#include "Core/Resource/Interface/Resource.h"
#include "Core/Graphics/Program.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceGroup;

EGE_DECLARE_SMART_CLASS(ResourceProgram, PResourceProgram)
EGE_DECLARE_SMART_CLASS(Shader, PShader)
EGE_DECLARE_SMART_CLASS(Object, PObject)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceProgram : public IResource
{
  public:

    virtual ~ResourceProgram();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Creates instance of resource. This method is a registration method for manager. */
    static PResource Create(Engine& engine, ResourceGroup* group);

    /*! @see IResource::name. */
    const String& name() const override;
    /*! @see IResource::create. */ 
    EGEResult create(const String& path, const PObject& data) override;
    /*! @see IResource::load. */
    EGEResult load() override;
    /*! @see IResource::unload. */
    void unload() override;

    /*! Gets instance of program object defined by resource. */
    PProgram program() const { return m_program; }

  private:

    ResourceProgram(Engine& engine, ResourceGroup* group);
    /*! Adds shader reference. */
    EGEResult addShaderReference(const PXmlElement& tag);
    /*! Loads all dependencies.
     *  @return Returns EGE_SUCCESS if all dependencies are ready (loaded). EGE_WAIT if some dependencies are still being loaded. 
     *          Otherwise one of the error values.
     */
    EGEResult loadDependencies();

  private slots:

    /*! Slot called when shader request is done. */
    void onRequestComplete(PObject object);

  private:

    /*! Shaders map [shaderName, shaderObject]. */
    typedef Map<String, PShader> ShaderMap;

  private:

    /*! Name. */
    String m_name;
    /*! Program object created from resource. NULL if not created yet. */
    PProgram m_program;
    /*! Shader map. */
    ShaderMap m_shaders;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCEPROGRAM_H
