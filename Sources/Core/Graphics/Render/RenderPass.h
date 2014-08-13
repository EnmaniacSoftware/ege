#ifndef EGE_CORE_RENDERPASS_H
#define EGE_CORE_RENDERPASS_H

/** Class describing single render pass for a material. 
 */

#include "EGE.h"
#include "EGEGraphics.h"
#include "EGEColorTransform.h"
#include "EGESignal.h"
#include "Core/Graphics/TextureImage.h"
#include "Core/Graphics/Program.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(RenderPass, PRenderPass)
EGE_DECLARE_SMART_CLASS(Object, PObject)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RenderPass : public Object
{
  public:

    RenderPass();
   ~RenderPass();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public signals:

    // TAGE - binding to RenderComponent is required

    /*! Signal emitter when program has changed. 
     *  @param  renderPass  Render pass for which program has changed.
     */
    Signal1<PRenderPass> programChanged;

  operators:

    bool operator == (const RenderPass& other) const;
    bool operator != (const RenderPass& other) const;

  public:

    /*! Adds new texture. */
    EGEResult addTexture(PTextureImage texture);
    /*! Sets new texture at given index. Can only succeed when setting texture within range. */
    EGEResult setTexture(u32 index, PTextureImage texture);
    /*! Sets new texture at the place of the one with given name. If no such texture exists it is added. */
    EGEResult setTexture(const String& name, PTextureImage texture);
    /*! Returns number of textures used. */
    u32 textureCount() const;
    /*! Retrives texture at given index. */
    PTextureImage texture(u32 index) const;
    /*! Remove texture at given index. 
     *  @param index Index of texture to be removed.
     *  @note  if negative index is passed all textures are removed.
     */
    void removeTexture(s32 index);
    
    /*! Sets source pixel blend factor. */
    void setSrcBlendFactor(EGEGraphics::BlendFactor factor);
    /*! Returns source pixel blend factor. */
    EGEGraphics::BlendFactor srcBlendFactor() const;
    /*! Sets destination pixel blend factor. */
    void setDstBlendFactor(EGEGraphics::BlendFactor factor);
    /*! Returns destination pixel blend factor. */
    EGEGraphics::BlendFactor dstBlendFactor() const;

    /*! Sets diffuse color base. */
    void setDiffuseColor(const Color& color);
    /*! Returns diffuse color base. */
    const Color& diffuseColor() const;
    /*! Returns diffuse color base. */
    Color& diffuseColor();
    /*! Sets ambient color. */
    void setAmbientColor(const Color& color);
    /*! Returns ambient color. */
    const Color& ambientColor() const;
    /*! Sets specular color. */
    void setSpecularColor(const Color& color);
    /*! Returns specular color. */
    const Color& specularColor() const;
    /*! Sets shininess. */
    void setShininess(float32 shininess);
    /*! Returns shininess. */
    float32 shininess() const;
    /*! Sets emission color. */
    void setEmissionColor(const Color& color);
    /*! Returns emission color. */
    const Color& emissionColor() const;

    /*! Sets GPU program. */
    void setProgram(const PProgram& program);
    /*! Returns GPU program. */
    PProgram program() const;

    /*! Sets diffuse color transformation. */
    void setDiffuseColorTransformation(const ColorTransform& transformation);
    /*! Returns current diffuse color transformation. */
    const ColorTransform& diffuseColorTransformation() const;
    /*! Returns cloned instance of this object. */
    PRenderPass clone() const;

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
    /*! Diffuse color transform. */
    ColorTransform m_diffuseColorTransform;
    /*! Specular color. */
    Color m_specularColor;
    /*! Emission color. */
    Color m_emissionColor;
    /*! Shininess value. */
    float32 m_shininess;
    /*! GPU program to use. */
    PProgram m_program;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline EGEGraphics::BlendFactor RenderPass::srcBlendFactor() const 
{ 
  return m_srcBlendFactor; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline EGEGraphics::BlendFactor RenderPass::dstBlendFactor() const 
{ 
  return m_dstBlendFactor; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline const Color& RenderPass::diffuseColor() const 
{ 
  return m_diffuseColor; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline Color& RenderPass::diffuseColor() 
{ 
  return m_diffuseColor; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline const Color& RenderPass::ambientColor() const 
{ 
  return m_ambientColor; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline const Color& RenderPass::specularColor() const 
{ 
  return m_specularColor; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline float32 RenderPass::shininess() const 
{ 
  return m_shininess; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline const Color& RenderPass::emissionColor() const 
{ 
  return m_emissionColor; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline const ColorTransform& RenderPass::diffuseColorTransformation() const 
{ 
  return m_diffuseColorTransform; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline PProgram RenderPass::program() const
{
  return m_program;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RENDERPASS_H