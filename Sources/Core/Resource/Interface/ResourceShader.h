#ifndef EGE_CORE_RESOURCESHADER_H
#define EGE_CORE_RESOURCESHADER_H

/** Shader resource definition class. This object (resource) contains definition of shader object.
 *  Shader resource can define any type of shader i.e. VERTEX or FRAGMENT. Upon loading it contains shader object to be used in rendering.
 */

#include "EGE.h"
#include "EGEString.h"
#include "EGEXml.h"
#include "EGEShader.h"
#include "Core/Resource/Interface/Resource.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceGroup;

EGE_PREDECLARE_SMART_CLASS(ResourceShader, PResourceShader)
EGE_PREDECLARE_SMART_CLASS(DataBuffer, PDataBuffer)
EGE_PREDECLARE_SMART_CLASS(Object, PObject)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceShader : public IResource
{
  public:

    ResourceShader(Engine& engine, ResourceGroup* group);
    virtual ~ResourceShader();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! @see IResource::name. */
    const String& name() const override;
    /*! @see IResource::create. */ 
    EGEResult create(const String& path, const PObject& data) override;
    /*! @see IResource::load. */
    EGEResult load() override;
    /*! @see IResource::unload. */
    void unload() override;

    /*! Gets instance of shader object defined by resource. */
    PShader shader() const { return m_shader; }

  private:

    /*! Creates shader. */
    EGEResult create();

  private slots:

    /*! Slot called when shader request is done. */
    void onRequestComplete(PObject object);

  private:

    /*! Name. */
    String m_name;
    /*! Shader type. */
    EGEGraphics::ShaderType m_type;
    /*! Shader object created from resource. NULL if not created yet. */
    PShader m_shader;
    /*! Shader source data. */
    PDataBuffer m_data;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCESHADER_H
