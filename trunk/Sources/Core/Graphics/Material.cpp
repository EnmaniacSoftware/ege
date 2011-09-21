#include "Core/Graphics/Material.h"
#include "Core/Data/DataBuffer.h"
#include "Core/Graphics/TextureImage.h"
#include "Core/Math/Math.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(Material)
EGE_DEFINE_DELETE_OPERATORS(Material)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Material::Material(Application* app) : Object(app)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Material::~Material()
{
  clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns pass at given index. */
PRenderPass Material::pass(u32 index) const
{
  return (index < passCount()) ? m_passes[index] : NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds render pass to material. 
 *  @param pass RenderPass obejct to be added.
 *  @note If NULL object passed in, new render pass object will be created and added.
 *  @return Return added object. NULL if error occured.
 */
PRenderPass Material::addPass(const PRenderPass& pass)
{
  RenderPass* renderPass = pass;

  // check if existing one should be added
  if (renderPass)
  {
    m_passes.push_back(renderPass);
  }
  else
  {
    // create new
    renderPass = ege_new RenderPass(app());
    if (renderPass)
    {
      m_passes.push_back(renderPass);
    }
  }

  return renderPass;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if material is valid. */
bool Material::isValid() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets source pixel scale factor for all passes. */
void Material::setSrcBlendFactor(EGEGraphics::BlendFactor factor)
{
  for (PassArray::iterator it = m_passes.begin(); it != m_passes.end(); ++it)
  {
    PRenderPass& pass = *it;

    pass->setSrcBlendFactor(factor);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets destination pixel scale factor for all passes. */
void Material::setDstBlendFactor(EGEGraphics::BlendFactor factor)
{
  for (PassArray::iterator it = m_passes.begin(); it != m_passes.end(); ++it)
  {
    PRenderPass& pass = *it;

    pass->setDstBlendFactor(factor);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets diffuse color for all passes. */
void Material::setDiffuseColor(const Color& color)
{
  for (PassArray::iterator it = m_passes.begin(); it != m_passes.end(); ++it)
  {
    PRenderPass& pass = *it;

    pass->setDiffuseColor(color);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets ambient color for all passes. */
void Material::setAmbientColor(const Color& color)
{
  for (PassArray::iterator it = m_passes.begin(); it != m_passes.end(); ++it)
  {
    PRenderPass& pass = *it;

    pass->setAmbientColor(color);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets specular color for all passes. */
void Material::setSpecularColor(const Color& color)
{
  for (PassArray::iterator it = m_passes.begin(); it != m_passes.end(); ++it)
  {
    PRenderPass& pass = *it;

    pass->setSpecularColor(color);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets shininess for all passes. */
void Material::setShininess(float32 shininess)
{
  for (PassArray::iterator it = m_passes.begin(); it != m_passes.end(); ++it)
  {
    PRenderPass& pass = *it;

    pass->setShininess(shininess);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets emission color for all passes. */
void Material::setEmissionColor(const Color& color)
{
  for (PassArray::iterator it = m_passes.begin(); it != m_passes.end(); ++it)
  {
    PRenderPass& pass = *it;

    pass->setEmissionColor(color);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Clears material. */
void Material::clear()
{
  m_passes.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
