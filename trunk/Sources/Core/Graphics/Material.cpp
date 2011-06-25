#include "Core/Graphics/Material.h"
#include "Core/Data/DataBuffer.h"
#include "Core/Math/Math.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(Material)
EGE_DEFINE_DELETE_OPERATORS(Material)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Material::Material(Application* app) : Object(app), m_diffuseColor(Color::WHITE), m_ambientColor(Color::WHITE), m_specularColor(Color::BLACK), m_shinness(0), 
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
/*! Sets shinness. */
void Material::setShinness(float32 shinness)
{
  m_shinness = Math::Bound(shinness, 0.0f, 1.0f);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets emission color. */
void Material::setEmissionColor(const Color& color)
{
  m_emissionColor = color;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
