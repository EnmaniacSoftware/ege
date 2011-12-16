#include "Core/Graphics/Render/RenderPass.h"
#include "Core/Graphics/TextureImage.h"
#include "Core/Math/Math.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(RenderPass)
EGE_DEFINE_DELETE_OPERATORS(RenderPass)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderPass::RenderPass(Application* app) : Object(app), 
                                           m_diffuseColor(Color::WHITE), 
                                           m_ambientColor(Color::WHITE), 
                                           m_specularColor(Color::BLACK), 
                                           m_shininess(0), 
                                           m_emissionColor(Color::BLACK), 
                                           m_srcBlendFactor(EGEGraphics::BF_ONE), 
                                           m_dstBlendFactor(EGEGraphics::BF_ZERO)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderPass::~RenderPass()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds new texture. */
EGEResult RenderPass::addTexture(PObject texture)
{
  m_textures.push_back(texture);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets new texture at given index. Can only succeed when setting texture within range. */
EGEResult RenderPass::setTexture(u32 index, PObject texture)
{
  // check if index out of range
  if (index >= static_cast<u32>(m_textures.size()))
  {
    // error!
    return EGE_ERROR;
  }

  // set new texture
  m_textures[index] = texture;
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets new texture at the place of the one with given name. If no such texture exists it is added. */
EGEResult RenderPass::setTexture(const String& name, PObject texture)
{
  // find texture with given name
  for (DynamicArray<PObject>::iterator it = m_textures.begin(); it != m_textures.end(); ++it)
  {
    // check if texture image
    if (EGE_OBJECT_UID_TEXTURE_IMAGE == (*it)->uid())
    {
      PTextureImage texImg = *it;
      if (texImg->name() == name)
      {
        EGE_ASSERT(EGE_OBJECT_UID_TEXTURE_IMAGE == texture->uid());

        // make a copy here
        texImg->copy((TextureImage*) texture.object());

        // make sure name stays the same
        texImg->setName(name);

        return EGE_SUCCESS;
      }
    }
  }

  // not found, append
  return addTexture(texture);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Remove texture at given index. 
 *  @param index Index of texture to be removed.
 *  @note  if index is -1 all textures are removed.
 */
void RenderPass::removeTexture(s32 index)
{
  if (0 > index)
  {
    m_textures.clear();
  }
  else if (index < static_cast<s32>(m_textures.size()))
  {
    m_textures.removeAt(index);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns number of textures used. */
u32 RenderPass::textureCount() const
{
  return (u32) m_textures.size();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Retrives texture at given index. */
PObject RenderPass::texture(u32 index) const
{
  return (index < textureCount()) ? m_textures[index] : NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets source pixel scale factor. */
void RenderPass::setSrcBlendFactor(EGEGraphics::BlendFactor factor)
{
  m_srcBlendFactor = factor;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets destination pixel scale factor. */
void RenderPass::setDstBlendFactor(EGEGraphics::BlendFactor factor)
{
  m_dstBlendFactor = factor;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets diffuse color. */
void RenderPass::setDiffuseColor(const Color& color)
{
  m_diffuseColor = color;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets ambient color. */
void RenderPass::setAmbientColor(const Color& color)
{
  m_ambientColor = color;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets specular color. */
void RenderPass::setSpecularColor(const Color& color)
{
  m_specularColor = color;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets shininess. */
void RenderPass::setShininess(float32 shininess)
{
  m_shininess = Math::Bound(shininess, 0.0f, 1.0f);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets emission color. */
void RenderPass::setEmissionColor(const Color& color)
{
  m_emissionColor = color;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns cloned instance of this object. */
RenderPass* RenderPass::clone() const
{
  RenderPass* pass = ege_new RenderPass(app());
  if (pass)
  {
    // TAGE - probably there is no need to clone textures too unless we need to modify textures as well
    for (ObjectArray::const_iterator it = m_textures.begin(); it != m_textures.end(); ++it)
    {
      pass->m_textures.push_back(*it);
    }

    pass->m_srcBlendFactor = m_srcBlendFactor;
    pass->m_dstBlendFactor = m_dstBlendFactor;
    pass->m_ambientColor   = m_ambientColor;
    pass->m_diffuseColor   = m_diffuseColor;
    pass->m_specularColor  = m_specularColor;
    pass->m_emissionColor  = m_emissionColor;
    pass->m_shininess      = m_shininess;
  }

  return pass;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
