#ifndef EGE_CORE_MATERIAL_H
#define EGE_CORE_MATERIAL_H

#include "EGE.h"
#include "EGEString.h"

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
    virtual ~Material();

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

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(Material);

    /*! Textures assigned to material. */
    std::vector<PObject> m_textures;
    /*! Blending flag. */
    bool m_blendingEnabled;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATERIAL_H
