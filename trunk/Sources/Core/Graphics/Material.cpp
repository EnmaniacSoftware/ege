#include "Core/Graphics/Material.h"
#include "Core/Data/DataBuffer.h"
#include "Core/Graphics/TextureImage.h"
#include "Core/Math/Math.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(Material)
EGE_DEFINE_DELETE_OPERATORS(Material)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Material::Material(Application* app) : Object(app), m_diffuseColor(Color::WHITE), m_ambientColor(Color::WHITE), m_specularColor(Color::BLACK), m_shininess(0), 
                                       m_emissionColor(Color::BLACK), m_srcBlendFactor(EGEGraphics::BLEND_FACTOR_ONE),
                                       m_dstBlendFactor(EGEGraphics::BLEND_FACTOR_ZERO)

{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Material::~Material()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds new texture. */
EGEResult Material::addTexture(PObject texture)
{
  m_textures.push_back(texture);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets new texture at given index. Can only succeed when setting texture within range. */
EGEResult Material::setTexture(u32 index, PObject texture)
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
EGEResult Material::setTexture(const String& name, PObject texture)
{
  // find texture with given name
  for (DynamicArray<PObject>::iterator it = m_textures.begin(); it != m_textures.end(); ++it)
  {
    bool found = false;
  
    if (EGE_OBJECT_UID_TEXTURE_IMAGE == (*it)->uid())
    {
      PTextureImage texImg = *it;
      if (texImg->name() == name)
      {
        found = true;
      }
    }

    // check if found
    if (found)
    {
      *it = texture;
      return EGE_SUCCESS;
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
void Material::removeTexture(s32 index)
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
u32 Material::textureCount() const
{
  return (u32) m_textures.size();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Retrives texture at given index. */
PObject Material::texture(u32 index) const
{
  return (index < textureCount()) ? m_textures[index] : NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if material is valid. */
bool Material::isValid() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets source pixel scale factor. */
void Material::setSrcBlendFactor(EGEGraphics::EBlendFactor factor)
{
  m_srcBlendFactor = factor;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets destination pixel scale factor. */
void Material::setDstBlendFactor(EGEGraphics::EBlendFactor factor)
{
  m_dstBlendFactor = factor;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets diffuse color. */
void Material::setDiffuseColor(const Color& color)
{
  m_diffuseColor = color;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets ambient color. */
void Material::setAmbientColor(const Color& color)
{
  m_ambientColor = color;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets specular color. */
void Material::setSpecularColor(const Color& color)
{
  m_specularColor = color;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets shininess. */
void Material::setShininess(float32 shininess)
{
  m_shininess = Math::Bound(shininess, 0.0f, 1.0f);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets emission color. */
void Material::setEmissionColor(const Color& color)
{
  m_emissionColor = color;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
