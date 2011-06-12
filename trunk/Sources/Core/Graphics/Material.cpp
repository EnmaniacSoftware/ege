#include "Core/Graphics/Material.h"
#include "Core/Data/DataBuffer.h"
#include "Core/Math/Math.h"

#if EGE_RENDERING_OPENGL_2 || EGE_RENDERING_OPENGLES_1
#include "Core/Graphics/OpenGL/MaterialOGL_p.h"
#endif // EGE_RENDERING_OPENGL_2 || EGE_RENDERING_OPENGLES_1

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(Material)
EGE_DEFINE_DELETE_OPERATORS(Material)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Material::Material(Application* app) : Object(app), m_blendingEnabled(false), m_diffuseColor(Color::WHITE), m_ambientColor(Color::WHITE), 
                                       m_specularColor(Color::BLACK), m_shinness(0), m_emissionColor(Color::BLACK)
{
  m_p = ege_new MaterialPrivate(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Material::~Material()
{
  EGE_DELETE(m_p);
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
  return NULL != m_p;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Enables/disables blending. */
void Material::enableBlending(bool enable)
{
  m_blendingEnabled = enable;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets source pixel scale function. */
void Material::setSrcBlendFunc(const EGEString& funcName)
{
  if (isValid())
  {
    p_func()->setSrcBlendFunc(funcName);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets destination pixel scale function. */
void Material::setDstBlendFunc(const EGEString& funcName)
{
  if (isValid())
  {
    p_func()->setDstBlendFunc(funcName);
  }
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
