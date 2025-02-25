#include "Core/Graphics/Render/RenderPass.h"
#include "Core/Graphics/TextureImage.h"
#include "EGEMath.h"

EGE_NAMESPACE_BEGIN

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
  m_textures.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult RenderPass::addTexture(PTextureImage texture)
{
  m_textures.push_back(texture);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult RenderPass::setTexture(u32 index, PTextureImage texture)
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
EGEResult RenderPass::setTexture(const String& name, PTextureImage texture)
{
  // find texture with given name
  for (TextureImageArray::iterator it = m_textures.begin(); it != m_textures.end(); ++it)
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
u32 RenderPass::textureCount() const
{
  return (u32) m_textures.size();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PTextureImage RenderPass::texture(u32 index) const
{
  return (index < textureCount()) ? m_textures[index] : NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderPass::setSrcBlendFactor(EGEGraphics::BlendFactor factor)
{
  m_srcBlendFactor = factor;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderPass::setDstBlendFactor(EGEGraphics::BlendFactor factor)
{
  m_dstBlendFactor = factor;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderPass::setDiffuseColor(const Color& color)
{
  m_diffuseColor = color;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderPass::setAmbientColor(const Color& color)
{
  m_ambientColor = color;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderPass::setSpecularColor(const Color& color)
{
  m_specularColor = color;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderPass::setShininess(float32 shininess)
{
  m_shininess = Math::Clamp(shininess, 0.0f, 1.0f);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderPass::setEmissionColor(const Color& color)
{
  m_emissionColor = color;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PRenderPass RenderPass::clone() const
{
  PRenderPass pass = ege_new RenderPass(app());
  if (NULL != pass)
  {
    // clone texture images
    for (TextureImageArray::const_iterator it = m_textures.begin(); it != m_textures.end(); ++it)
    {
      const TextureImage& currentTextureImage = *it->object();

      PTextureImage textureImage = ege_new TextureImage(currentTextureImage);
      pass->m_textures.push_back(textureImage);
    }

    pass->m_srcBlendFactor        = m_srcBlendFactor;
    pass->m_dstBlendFactor        = m_dstBlendFactor;
    pass->m_ambientColor          = m_ambientColor;
    pass->m_diffuseColor          = m_diffuseColor;
    pass->m_specularColor         = m_specularColor;
    pass->m_emissionColor         = m_emissionColor;
    pass->m_shininess             = m_shininess;
    pass->m_diffuseColorTransform = m_diffuseColorTransform;
    pass->m_program               = m_program;
  }

  return pass;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderPass::setDiffuseColorTransformation(const ColorTransform& transformation)
{
  m_diffuseColorTransform = transformation;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderPass::setProgram(const PProgram& program)
{
  // NOTE: pointer check
  if (m_program != program)
  {
    m_program = program;

    // emit
    emit programChanged(this);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool RenderPass::operator == (const RenderPass& other) const
{
  bool result = false;

  // check if same blending factors, number of textures, same colors
  if ((m_srcBlendFactor == other.m_srcBlendFactor) && (m_dstBlendFactor == other.m_dstBlendFactor) && (m_textures.size() == other.m_textures.size()) &&
      (m_ambientColor == other.m_ambientColor) && (m_diffuseColor == other.m_diffuseColor) && (m_diffuseColorTransform == other.m_diffuseColorTransform) &&
      (m_emissionColor == other.m_emissionColor) && (m_shininess == other.m_shininess))
  {
    result = true;

    // check if textures are different
    for (s32 i = 0; i < static_cast<s32>(m_textures.size()); ++i)
    {
      // NOTE: assuptions:
      //       - textures can be compared by pointer
      //       - texture images has same environment modes
      //       - textues has the same rotation
      if ((m_textures[i]->texture() != other.m_textures[i]->texture()) || 
          (m_textures[i]->environmentMode() != other.m_textures[i]->environmentMode()) ||
          (m_textures[i]->rotationAngle() != other.m_textures[i]->rotationAngle()))
      {
        // different
        result = false;

        // done
        break;
      }
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool RenderPass::operator != (const RenderPass& other) const
{
  return ! (*this == other);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END