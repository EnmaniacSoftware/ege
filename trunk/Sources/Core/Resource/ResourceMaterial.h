#ifndef EGE_CORE_RESOURCEMATERIAL_H
#define EGE_CORE_RESOURCEMATERIAL_H

/**  Material resource definition class. This object (resource) contains definition of render material.
 *   Upon loading it contains material object to be used in rendering. Materials consist of one or more render passes.
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
class ResourceGroup;

EGE_DECLARE_SMART_CLASS(ResourceMaterial, PResourceMaterial)
EGE_DECLARE_SMART_CLASS(TextureImage, PTextureImage)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceMaterial : public IResource
{
  private:

    typedef List<PTextureImage> TextureImageList;

  private:

    /*! Small structure containing information for TextureImage. */
    struct TextureImageData
    {
        String name;                                /*!< Texture name. */
        Rectf rect;                                 /*!< Texture rectangle (in normalized local coords). */
        EGETexture::EnvironmentMode envMode;        /*!< Texture environment mode. */
        bool manual;                                /*!< Manual flag. */
        Angle rotationAngle;                        /*!< Texture rotation angle. */
        PTextureImage textureImage;                 /*!< Loaded texture image. It is loaded and initialized with data above. */
    };
        
    typedef List<TextureImageData> TextureImageDataList;

    /*! Small class containing information about render passes. */
    class PassData
    {
      public:

        PassData() : m_srcBlend(EGEGraphics::BF_ONE), m_dstBlend(EGEGraphics::BF_ZERO), m_ambientColor(Color::WHITE), m_diffuseColor(Color::WHITE), 
                     m_specularColor(Color::BLACK), m_emissionColor(Color::BLACK), m_shininess(0) {}

      public:

        EGEGraphics::BlendFactor m_srcBlend;            /*!< Source blend value. */
        EGEGraphics::BlendFactor m_dstBlend;            /*!< Destination blend value. */
        Color m_ambientColor;                           /*!< Ambient color. */
        Color m_diffuseColor;                           /*!< Diffuse color. */
        Color m_specularColor;                          /*!< Specular color. */
        Color m_emissionColor;                          /*!< Emission color. */
        float32 m_shininess;                            /*!< Shininess value. */
        TextureImageList m_textureImages;               /*!< List of all loaded texture image objects used by pass. */
        TextureImageDataList m_textureImageData;        /*!< List of definitions of all texture images contributing to pass. */
    };

  public:

    virtual ~ResourceMaterial();

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

    /*! Creates instance of material object defined by resource. */
    PMaterial createInstance() const;
    /*! Set given instance of material object to what is defined by resource. */
    EGEResult setInstance(const PMaterial& instance) const;

    /*! Returns source pixel factor function for a given pass. */
    EGEGraphics::BlendFactor srcBlendFactor(u32 pass) const;
    /*! Returns destination pixel factor function for a given pass. */
    EGEGraphics::BlendFactor dstBlendFactor(u32 pass) const;
    /*! Returns ambient color for a given pass. */
    const Color& ambientColor(u32 pass) const;
    /*! Returns diffuse color for a given pass. */
    const Color& diffuseColor(u32 pass) const;
    /*! Returns specular color for a given pass. */
    const Color& specularColor(u32 pass) const;
    /*! Returns emission color for a given pass. */
    const Color& emissionColor(u32 pass) const;
    /*! Returns shinness value for a given pass. */
    float32 shininess(u32 pass) const;
    /*! Returns number of passes. */
    const u32 passCount() const { return static_cast<u32>(m_passes.size()); }

  private:

    ResourceMaterial(Application* app, ResourceGroup* group);
    /*! Adds texture dependancy to given pass. */
    EGEResult addTexture(const PXmlElement& tag, PassData& pass);
    /*! Adds pass. */
    EGEResult addPass(const PXmlElement& tag);
    /*! Loads all dependencies.
     *  @return Returns EGE_SUCCESS if all dependencies are ready (loaded). EGE_WAIT if some dependencies are still being loaded. 
     *          Otherwise one of the error values.
     */
    EGEResult loadDependencies();

  private:

    typedef DynamicArray<PassData> PassDataArray;

  private:

    /*! Name. */
    String m_name;
    /*! List of all passes. */
    PassDataArray m_passes;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCEMATERIAL_H
