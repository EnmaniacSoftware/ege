#ifndef EGE_CORE_RESOURCEMATERIAL_H
#define EGE_CORE_RESOURCEMATERIAL_H

#include "EGE.h"
#include "EGEString.h"
#include "EGEXml.h"
#include "Core/Resource/Resource.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceManager;

EGE_DECLARE_SMART_CLASS(ResourceMaterial, PResourceMaterial)
EGE_DECLARE_SMART_CLASS(Material, PMaterial)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceMaterial : public IResource
{
  public:

    virtual ~ResourceMaterial();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Creates instance of resource. This method is a registration method for manager. */
    static PResource Create(Application* app, ResourceManager* manager);

    /* IResource override. Returns name of resource. */
    const EGEString& name() const override;
    /* Initializes resource from XML. 
    * 
    *  \param  path  full path to resource definition file.
    *  \param  tag   xml element with resource definition. 
    */
    EGEResult create(const EGEString& path, const PXmlElement& tag) override;
    /* IResource override. Loads resource. */
    EGEResult load() override;
    /* IResource override. Unloads resource. */
    void unload() override;

    /*! Gets instance of material object defined by resource. */
    inline PMaterial material() const { return m_material; }

  private:

    ResourceMaterial(Application* app, ResourceManager* manager);
    /* Adds texture dependancy. */
    EGEResult addTexture(const PXmlElement& tag);
    /*! Returns TRUE if material is loaded. */
    inline bool isLoaded() const { return NULL != m_material; }
    /*! Returns source pixel scale function name. */
    inline const EGEString& srcBlendFuncName() const { return m_srcBlend; }
    /*! Returns destination pixel scale function name. */
    inline const EGEString& dstBlendFuncName() const { return m_dstBlend; }
    /*! Returns blending state name. */
    inline const EGEString& blendingEnabledName() const { return m_blend; }
    /*! Returns ambient color name. */
    inline const EGEString& ambientColorName() const { return m_ambientColor; }
    /*! Returns diffuse color name. */
    inline const EGEString& diffuseColorName() const { return m_diffuseColor; }
    /*! Returns specular color name. */
    inline const EGEString& specularColorName() const { return m_specularColor; }
    /*! Returns emission color name. */
    inline const EGEString& emissionColorName() const { return m_emissionColor; }
    /*! Returns shinness value name. */
    inline const EGEString& shinnessName() const { return m_shinness; }
    
  private:

    /*! Name. */
    EGEString m_name;
    /*! Texture names. */
    EGEStringList m_textureNames;
    /*! Source blend value name. */
    EGEString m_srcBlend;
    /*! Destination blend value name. */
    EGEString m_dstBlend;
    /*! Blending enabled value name. */
    EGEString m_blend;
    /*! Ambient color name. */
    EGEString m_ambientColor;
    /*! Diffuse color name. */
    EGEString m_diffuseColor;
    /*! Specular color name. */
    EGEString m_specularColor;
    /*! Emission color name. */
    EGEString m_emissionColor;
    /*! Shinness value name. */
    EGEString m_shinness;

    /*! Material object created from resource. NULL if not created yet. */
    PMaterial m_material;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCEMATERIAL_H
