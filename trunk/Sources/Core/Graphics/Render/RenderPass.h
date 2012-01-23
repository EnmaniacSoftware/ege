#ifndef EGE_CORE_RENDERPASS_H
#define EGE_CORE_RENDERPASS_H

/** Class describing single render pass for a material. 
 */

#include <EGE.h>
#include <EGEGraphics.h>
#include "Core/Graphics/TextureImage.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(RenderPass, PRenderPass)
EGE_DECLARE_SMART_CLASS(Object, PObject)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class RenderPass : public Object
{
  public:

    RenderPass(Application* app);
   ~RenderPass();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /* Adds new texture. */
    EGEResult addTexture(PTextureImage texture);
    /* Sets new texture at given index. Can only succeed when setting texture within range. */
    EGEResult setTexture(u32 index, PTextureImage texture);
    /* Sets new texture at the place of the one with given name. If no such texture exists it is added. */
    EGEResult setTexture(const String& name, PTextureImage texture);
    /* Returns number of textures used. */
    u32 textureCount() const;
    /* Retrives texture at given index. */
    PTextureImage texture(u32 index) const;
    /* Remove texture at given index. 
     * @param index Index of texture to be removed.
     * @note  if negative index is passed all textures are removed.
     */
    void removeTexture(s32 index);
    
    /* Sets source pixel blend factor. */
    void setSrcBlendFactor(EGEGraphics::BlendFactor factor);
    /*! Returns source pixel blend factor. */
    inline EGEGraphics::BlendFactor srcBlendFactor() const { return m_srcBlendFactor; }
    /* Sets destination pixel blend factor. */
    void setDstBlendFactor(EGEGraphics::BlendFactor factor);
    /*! Returns destination pixel blend factor. */
    inline EGEGraphics::BlendFactor dstBlendFactor() const { return m_dstBlendFactor; }

    /* Sets diffuse color. */
    void setDiffuseColor(const Color& color);
    /*! Returns diffuse color. */
    const Color& diffuseColor() const { return m_diffuseColor; }
    /*! Returns diffuse color. */
    Color& diffuseColor() { return m_diffuseColor; }
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

    /* Returns cloned instance of this object. */
    RenderPass* clone() const;

  private:

    typedef DynamicArray<PTextureImage> TextureImageArray;

  private:

    /*! Textures assigned to pass. */
    TextureImageArray m_textures;
    /*! Source blend value. */
    EGEGraphics::BlendFactor m_srcBlendFactor;
    /*! Destination blend value. */
    EGEGraphics::BlendFactor m_dstBlendFactor;
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
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RENDERPASS_H