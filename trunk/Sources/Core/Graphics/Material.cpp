#include "Core/Graphics/Material.h"
#include "Core/Data/DataBuffer.h"

#if EGE_RENDERING_OPENGL_2 || EGE_RENDERING_OPENGLES_1
#include "Core/Graphics/OpenGL/MaterialOGL_p.h"
#endif // EGE_RENDERING_OPENGL_2 || EGE_RENDERING_OPENGLES_1

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(Material)
EGE_DEFINE_DELETE_OPERATORS(Material)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Material::Material(Application* app) : Object(app), m_blendingEnabled(false)
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
