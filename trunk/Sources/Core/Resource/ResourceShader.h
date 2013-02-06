#ifndef EGE_CORE_RESOURCSHADER_H
#define EGE_CORE_RESOURCSHADER_H

/** Shader resource definition class. This object (resource) contains definition of shader object.
 *  Shader resource can define any type of shader i.e. VERTEX or FRAGMENT. Upon loading it contains shader object to be used in rendering.
 */

#include <EGE.h>
#include <EGEString.h>
#include <EGEXml.h>
#include <EGEShader.h>
#include "Core/Resource/Resource.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceGroup;

EGE_DECLARE_SMART_CLASS(ResourceShader, PResourceShader)
EGE_DECLARE_SMART_CLASS(DataBuffer, PDataBuffer)
EGE_DECLARE_SMART_CLASS(Object, PObject)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceShader : public IResource
{
  public:

    virtual ~ResourceShader();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Creates instance of resource. This method is a registration method for manager. */
    static PResource Create(Application* app, ResourceGroup* group);

    /*! @see IResource::name. */
    const String& name() const override;
    /*! @see IResource::create. */ 
    EGEResult create(const String& path, const PXmlElement& tag) override;
    /*! @see IResource::load. */
    EGEResult load() override;
    /*! @see IResource::unload. */
    void unload() override;

    /*! Gets instance of shader object defined by resource. */
    PShader shader() const { return m_shader; }

  private:

    ResourceShader(Application* app, ResourceGroup* group);
    /*! Creates shader. */
    EGEResult create();

  private slots:

    /*! Slot called when shader request is done. */
    void onRequestComplete(u32 handle, PObject object);

  private:

    /*! Name. */
    String m_name;
    /*! Shader type. */
    EGEGraphics::ShaderType m_type;
    /*! Shader object created from resource. NULL if not created yet. */
    PShader m_shader;
    /*! Resource request id. */
    u32 m_resourceRequestId;
    /*! Shader source data. */
    PDataBuffer m_data;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCSHADER_H
