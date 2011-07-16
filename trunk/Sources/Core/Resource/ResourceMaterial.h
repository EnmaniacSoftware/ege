#ifndef EGE_CORE_RESOURCEMATERIAL_H
#define EGE_CORE_RESOURCEMATERIAL_H

/** Material resource definition class. This object (resource) contains definition of render material.
 *   Upon loading it contains material object to be used in rendering.
 */

#include <EGE.h>
#include <EGEString.h>
#include <EGEXml.h>
#include <EGEMath.h>
#include <EGEList.h>
#include <EGEGraphics.h>
#include "Core/Resource/Resource.h"
#include "Core/Graphics/Material.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceManager;

EGE_DECLARE_SMART_CLASS(ResourceMaterial, PResourceMaterial)

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
    const String& name() const override;
    /* Initializes resource from XML. 
    * 
    *  \param  path  full path to resource definition file.
    *  \param  tag   xml element with resource definition. 
    */
    EGEResult create(const String& path, const PXmlElement& tag) override;
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
    /* Adds texture image dependancy. */
    EGEResult addTextureImage(const PXmlElement& tag);
    /*! Returns TRUE if material is loaded. */
    inline bool isLoaded() const { return NULL != m_material; }
    /*! Returns source pixel factor function. */
    inline EGEGraphics::EBlendFactor srcBlendFactor() const { return m_srcBlend; }
    /*! Returns destination pixel factor function. */
    inline EGEGraphics::EBlendFactor dstBlendFactor() const { return m_dstBlend; }
    /*! Returns ambient color name. */
    inline const String& ambientColorName() const { return m_ambientColor; }
    /*! Returns diffuse color name. */
    inline const String& diffuseColorName() const { return m_diffuseColor; }
    /*! Returns specular color name. */
    inline const String& specularColorName() const { return m_specularColor; }
    /*! Returns emission color name. */
    inline const String& emissionColorName() const { return m_emissionColor; }
    /*! Returns shinness value name. */
    inline const String& shinnessName() const { return m_shinness; }
    /* Maps blend factor's name into value. */
    EGEGraphics::EBlendFactor mapBlendFactor(const String& name, EGEGraphics::EBlendFactor defaultValue) const;
    
  private:

    /*! Small class containing information for TextureImage. */
    class TextureImageData
    {
      public:

        TextureImageData(const String& textureName, const Rectf& textureRect) : m_textureName(textureName), m_textureRect(textureRect) {}

      public:

        /*! Texture name. */
        String m_textureName;
        /*! Texture rectangle. */
        Rectf m_textureRect;
    };

  private:

    /*! Name. */
    String m_name;
    /*! List of all texture images contributing to material. */
    List<TextureImageData> m_textureImages;
    /*! Source blend value. */
    EGEGraphics::EBlendFactor m_srcBlend;
    /*! Destination blend value. */
    EGEGraphics::EBlendFactor m_dstBlend;
    /*! Ambient color name. */
    String m_ambientColor;
    /*! Diffuse color name. */
    String m_diffuseColor;
    /*! Specular color name. */
    String m_specularColor;
    /*! Emission color name. */
    String m_emissionColor;
    /*! Shinness value name. */
    String m_shinness;

    /*! Material object created from resource. NULL if not created yet. */
    PMaterial m_material;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCEMATERIAL_H
