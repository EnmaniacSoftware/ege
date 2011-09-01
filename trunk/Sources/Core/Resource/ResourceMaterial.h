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
#include <EGETexture.h>
#include "Core/Resource/Resource.h"
#include "Core/Graphics/Material.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceManager;

EGE_DECLARE_SMART_CLASS(ResourceMaterial, PResourceMaterial)
EGE_DECLARE_SMART_CLASS(TextureImage, PTextureImage)

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

    /* Creates instance of material object defined by resource. */
    PMaterial createInstance() const;
    /* Set given instance of material object to what is defined by resource. */
    EGEResult setInstance(const PMaterial& instance) const;

  private:

    ResourceMaterial(Application* app, ResourceManager* manager);
    /* Adds texture dependancy. */
    EGEResult addTexture(const PXmlElement& tag);
    /*! Returns TRUE if material is loaded. */
    inline bool isLoaded() const { return !m_textureImages.empty(); }
    /*! Returns source pixel factor function. */
    inline EGEGraphics::BlendFactor srcBlendFactor() const { return m_srcBlend; }
    /*! Returns destination pixel factor function. */
    inline EGEGraphics::BlendFactor dstBlendFactor() const { return m_dstBlend; }
    /*! Returns ambient color. */
    inline const Color& ambientColor() const { return m_ambientColor; }
    /*! Returns diffuse color. */
    inline const Color& diffuseColor() const { return m_diffuseColor; }
    /*! Returns specular color. */
    inline const Color& specularColor() const { return m_specularColor; }
    /*! Returns emission color. */
    inline const Color& emissionColor() const { return m_emissionColor; }
    /*! Returns shinness value. */
    inline float32 shininess() const { return m_shininess; }
    
  private:

    /*! Small class containing information for TextureImage. */
    class TextureImageData
    {
      public:

        TextureImageData(const String& name, const Rectf& rect, EGETexture::EnvironmentMode envMode, bool manual) : m_name(name), m_rect(rect), 
                                                                                                                    m_envMode(envMode), m_manual(manual) {}

      public:

        /*! Texture name. */
        String m_name;
        /*! Texture rectangle (in normalized local coords). */
        Rectf m_rect;
        /*! Texture environment mode. */
        EGETexture::EnvironmentMode m_envMode;
        /*! Manual flag. */
        bool m_manual;
    };

  private:

    typedef List<PTextureImage> TextureImageList;
    typedef List<TextureImageData> TextureImageDataList;

  private:

    /*! Name. */
    String m_name;
    /*! List of all texture images contributing to material. */
    TextureImageDataList m_textureImageData;
    /*! Source blend value. */
    EGEGraphics::BlendFactor m_srcBlend;
    /*! Destination blend value. */
    EGEGraphics::BlendFactor m_dstBlend;
    /*! Ambient color. */
    Color m_ambientColor;
    /*! Diffuse color. */
    Color m_diffuseColor;
    /*! Specular color. */
    Color m_specularColor;
    /*! Emission color. */
    Color m_emissionColor;
    /*! Shininess value. */
    float32 m_shininess;
    /*! List of all texture image objects referred by material. Empty if material is not loaded yet. */
    TextureImageList m_textureImages;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCEMATERIAL_H
