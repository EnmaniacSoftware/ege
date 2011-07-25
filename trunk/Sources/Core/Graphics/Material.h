#ifndef EGE_CORE_MATERIAL_H
#define EGE_CORE_MATERIAL_H

#include <EGE.h>
#include <EGEString.h>
#include <EGEGraphics.h>
#include <EGEDynamicArray.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(Material, PMaterial)
EGE_DECLARE_SMART_CLASS(Object, PObject)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Material : public Object
{
  public:

    Material(Application* app);
   ~Material();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if material is valid. */
    bool isValid() const;

    /* Adds new texture. */
    EGEResult addTexture(PObject texture);
    /* Returns number of textures used. */
    u32 textureCount() const;
    /* Retrives texture at given index. */
    PObject texture(u32 index) const;
    
    /* Sets source pixel blend factor. */
    void setSrcBlendFactor(EGEGraphics::EBlendFactor factor);
    /*! Returns source pixel blend factor. */
    inline EGEGraphics::EBlendFactor srcBlendFactor() const { return m_srcBlendFactor; }
    /* Sets destination pixel blend factor. */
    void setDstBlendFactor(EGEGraphics::EBlendFactor factor);
    /*! Returns destination pixel blend factor. */
    inline EGEGraphics::EBlendFactor dstBlendFactor() const { return m_dstBlendFactor; }

    /* Sets diffuse color. */
    void setDiffuseColor(const Color& color);
    /*! Returns diffuse color. */
    const Color& diffuseColor() const { return m_diffuseColor; }
    /* Sets ambient color. */
    void setAmbientColor(const Color& color);
    /*! Returns ambient color. */
    const Color& ambientColor() const { return m_ambientColor; }
    /* Sets specular color. */
    void setSpecularColor(const Color& color);
    /*! Returns specular color. */
    const Color& specularColor() const { return m_specularColor; }
    /* Sets shininess. */
    void setShininess(float32 shininess);
    /*! Returns shininess. */
    float32 shininess() const { return m_shininess; }
    /* Sets emission color. */
    void setEmissionColor(const Color& color);
    /*! Returns emission color. */
    const Color& emissionColor() const { return m_emissionColor; }

  private:

    /*! Textures assigned to material. */
    DynamicArray<PObject> m_textures;
    /*! Diffuse color reflectance. */
    Color m_diffuseColor;
    /*! Ambient color reflectance. */
    Color m_ambientColor;
    /*! Specular color reflectance. */
    Color m_specularColor;
    /*! Shininess [0-1] range. */
    float32 m_shininess;
    /*! Emission (self-illumination) color. */
    Color m_emissionColor;
    /*! Source blend factor. */
    EGEGraphics::EBlendFactor m_srcBlendFactor;
    /*! Destination blend factor. */
    EGEGraphics::EBlendFactor m_dstBlendFactor;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATERIAL_H
