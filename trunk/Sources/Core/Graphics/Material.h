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
    /* For RendererPrivate. */
    friend class RendererPrivate;

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
    /* Enables/disables blending. */
    void enableBlending(bool enable);
    /* Returns TRUE if blending is enabled. */
    inline bool isBlendingEnabled() const { return m_blendingEnabled; }
    /* Sets source pixel scale function. */
    void setSrcBlendFunc(const EGEString& funcName);
    /* Sets destination pixel scale function. */
    void setDstBlendFunc(const EGEString& funcName);
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
    /* Sets shinness. */
    void setShinness(float32 shinness);
    /*! Returns shinness. */
    float32 shinness() const { return m_shinness; }
    /* Sets emission color. */
    void setEmissionColor(const Color& color);
    /*! Returns emission color. */
    const Color& emissionColor() const { return m_emissionColor; }

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(Material);

    /*! Textures assigned to material. */
    EGEDynamicArray<PObject> m_textures;
    /*! Blending flag. */
    bool m_blendingEnabled;
    /*! Diffuse color reflectance. */
    Color m_diffuseColor;
    /*! Ambient color reflectance. */
    Color m_ambientColor;
    /*! Specular color reflectance. */
    Color m_specularColor;
    /*! Shinness [0-1] range. */
    float32 m_shinness;
    /*! Emission (self-illumination) color. */
    Color m_emissionColor;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATERIAL_H
