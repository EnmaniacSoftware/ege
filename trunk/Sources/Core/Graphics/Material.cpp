#include "Core/Graphics/Material.h"
#include "Core/Data/DataBuffer.h"
#include "Core/Graphics/TextureImage.h"
#include "Core/Math/Math.h"

EGE_NAMESPACE_BEGIN

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
PRenderPass Material::pass(u32 index) const
{
  return (index < passCount()) ? m_passes[index] : NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
bool Material::isValid() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Material::setSrcBlendFactor(EGEGraphics::BlendFactor factor)
{
  for (PassArray::iterator it = m_passes.begin(); it != m_passes.end(); ++it)
  {
    PRenderPass& pass = *it;

    pass->setSrcBlendFactor(factor);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Material::setDstBlendFactor(EGEGraphics::BlendFactor factor)
{
  for (PassArray::iterator it = m_passes.begin(); it != m_passes.end(); ++it)
  {
    PRenderPass& pass = *it;

    pass->setDstBlendFactor(factor);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Material::setDiffuseColor(const Color& color)
{
  for (PassArray::iterator it = m_passes.begin(); it != m_passes.end(); ++it)
  {
    PRenderPass& pass = *it;

    pass->setDiffuseColor(color);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Material::setDiffuseAlpha(float32 alpha)
{
  for (PassArray::iterator it = m_passes.begin(); it != m_passes.end(); ++it)
  {
    PRenderPass& pass = *it;

    pass->diffuseColor().alpha = alpha;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Material::setAmbientColor(const Color& color)
{
  for (PassArray::iterator it = m_passes.begin(); it != m_passes.end(); ++it)
  {
    PRenderPass& pass = *it;

    pass->setAmbientColor(color);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Material::setSpecularColor(const Color& color)
{
  for (PassArray::iterator it = m_passes.begin(); it != m_passes.end(); ++it)
  {
    PRenderPass& pass = *it;

    pass->setSpecularColor(color);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Material::setShininess(float32 shininess)
{
  for (PassArray::iterator it = m_passes.begin(); it != m_passes.end(); ++it)
  {
    PRenderPass& pass = *it;

    pass->setShininess(shininess);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Material::setEmissionColor(const Color& color)
{
  for (PassArray::iterator it = m_passes.begin(); it != m_passes.end(); ++it)
  {
    PRenderPass& pass = *it;

    pass->setEmissionColor(color);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Material::clear()
{
  m_passes.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PMaterial Material::clone() const
{
  PMaterial material = ege_new Material(app());
  if (NULL != material)
  {
    // clone passes
    for (PassArray::const_iterator it = m_passes.begin(); it != m_passes.end(); ++it)
    {
      RenderPass* clonedPass = (*it)->clone();
      if (NULL == clonedPass)
      {
        // error!
        return NULL;
      }

      material->addPass(clonedPass);
    }
  }

  return material;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END