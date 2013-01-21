#include "Core/Graphics/Material.h"

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
  return m_passes.at(index, NULL);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PRenderPass Material::addPass(const PRenderPass& pass)
{
  RenderPass* renderPass = pass;

  // check if existing one should be added
  if (NULL != renderPass)
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
  return ! m_passes.empty();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Material::setSrcBlendFactor(EGEGraphics::BlendFactor factor, s32 passIndex)
{
  PRenderPass pass = m_passes.at(passIndex, NULL);
  if (NULL != pass)
  {
    pass->setSrcBlendFactor(factor);
  }
  else if (0 > passIndex)
  {
    for (PassArray::iterator it = m_passes.begin(); it != m_passes.end(); ++it)
    {
      PRenderPass& pass = *it;

      pass->setSrcBlendFactor(factor);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Material::setDstBlendFactor(EGEGraphics::BlendFactor factor, s32 passIndex)
{
  PRenderPass pass = m_passes.at(passIndex, NULL);
  if (NULL != pass)
  {
    pass->setDstBlendFactor(factor);
  }
  else if (0 > passIndex)
  {
    for (PassArray::iterator it = m_passes.begin(); it != m_passes.end(); ++it)
    {
      PRenderPass& pass = *it;

      pass->setDstBlendFactor(factor);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Material::setDiffuseColor(const Color& color, s32 passIndex)
{
  PRenderPass pass = m_passes.at(passIndex, NULL);
  if (NULL != pass)
  {
    pass->setDiffuseColor(color);
  }
  else if (0 > passIndex)
  {
    for (PassArray::iterator it = m_passes.begin(); it != m_passes.end(); ++it)
    {
      PRenderPass& pass = *it;

      pass->setDiffuseColor(color);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Material::setDiffuseAlpha(float32 alpha, s32 passIndex)
{
  PRenderPass pass = m_passes.at(passIndex, NULL);
  if (NULL != pass)
  {
    pass->diffuseColor().alpha = alpha;
  }
  else if (0 > passIndex)
  {
    for (PassArray::iterator it = m_passes.begin(); it != m_passes.end(); ++it)
    {
      PRenderPass& pass = *it;

      pass->diffuseColor().alpha = alpha;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Material::setAmbientColor(const Color& color, s32 passIndex)
{
  PRenderPass pass = m_passes.at(passIndex, NULL);
  if (NULL != pass)
  {
    pass->setAmbientColor(color);
  }
  else if (0 > passIndex)
  {
    for (PassArray::iterator it = m_passes.begin(); it != m_passes.end(); ++it)
    {
      PRenderPass& pass = *it;

      pass->setAmbientColor(color);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Material::setSpecularColor(const Color& color, s32 passIndex)
{
  PRenderPass pass = m_passes.at(passIndex, NULL);
  if (NULL != pass)
  {
    pass->setSpecularColor(color);
  }
  else if (0 > passIndex)
  {
    for (PassArray::iterator it = m_passes.begin(); it != m_passes.end(); ++it)
    {
      PRenderPass& pass = *it;

      pass->setSpecularColor(color);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Material::setShininess(float32 shininess, s32 passIndex)
{
  PRenderPass pass = m_passes.at(passIndex, NULL);
  if (NULL != pass)
  {
    pass->setShininess(shininess);
  }
  else if (0 > passIndex)
  {
    for (PassArray::iterator it = m_passes.begin(); it != m_passes.end(); ++it)
    {
      PRenderPass& pass = *it;

      pass->setShininess(shininess);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Material::setEmissionColor(const Color& color, s32 passIndex)
{
  PRenderPass pass = m_passes.at(passIndex, NULL);
  if (NULL != pass)
  {
    pass->setEmissionColor(color);
  }
  else if (0 > passIndex)
  {
    for (PassArray::iterator it = m_passes.begin(); it != m_passes.end(); ++it)
    {
      PRenderPass& pass = *it;

      pass->setEmissionColor(color);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Material::setDiffuseColorTransformation(const ColorTransform& transformation, s32 passIndex)
{
  PRenderPass pass = m_passes.at(passIndex, NULL);
  if (NULL != pass)
  {
    pass->setDiffuseColorTransformation(transformation);
  }
  else if (0 > passIndex)
  {
    for (PassArray::iterator it = m_passes.begin(); it != m_passes.end(); ++it)
    {
      PRenderPass& pass = *it;

      pass->setDiffuseColorTransformation(transformation);
    }
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