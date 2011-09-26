#include "Core/Resource/ResourceMaterial.h"
#include "Core/Resource/ResourceTexture.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/ResourceTextureImage.h"
#include "Core/Graphics/Material.h"
#include "Core/Graphics/TextureImage.h"
#include <EGETexture.h>
#include <EGEResources.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define NODE_TEXTURE  "texture"
#define NODE_PASS     "pass"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function mapping texture environment mode name into value. */
EGETexture::EnvironmentMode MapTextureEnvironmentMode(const String& name, EGETexture::EnvironmentMode defaultValue)
{
  if ("replace" == name)
  {
    return EGETexture::EM_REPLACE;
  }
  else if ("modulate" == name)
  {
    return EGETexture::EM_MODULATE;
  }
  else if ("decal" == name)
  {
    return EGETexture::EM_DECAL;
  }
  else if ("blend" == name)
  {
    return EGETexture::EM_BLEND;
  }
  else if ("add" == name)
  {
    return EGETexture::EM_ADD;
  }
  else if ("combine" == name)
  {
    return EGETexture::EM_COMBINE;
  }

  return defaultValue;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function mapping blend factor's name into value. */
EGEGraphics::BlendFactor MapBlendFactor(const String& name, EGEGraphics::BlendFactor defaultValue)
{
  // check if no data to convert
  if (name.empty())
  {
    // return default value
    return defaultValue;
  }

  // map value
  EGEGraphics::BlendFactor factor = EGEGraphics::BF_UNKNOWN;

  if ("zero" == name)
  {
    factor = EGEGraphics::BF_ZERO;
  }
  else if ("one" == name)
  {
    factor = EGEGraphics::BF_ONE;
  }
  else if ("src-color" == name)
  {
    factor = EGEGraphics::BF_SRC_COLOR;
  }
  else if ("dst-color" == name)
  {
    factor = EGEGraphics::BF_DST_COLOR;
  }
  else if ("one-minus-src-color" == name)
  {
    factor = EGEGraphics::BF_ONE_MINUS_SRC_COLOR;
  }
  else if ("one-minus-dst-color" == name)
  {
    factor = EGEGraphics::BF_ONE_MINUS_DST_COLOR;
  }
  else if ("src-alpha" == name)
  {
    factor = EGEGraphics::BF_SRC_ALPHA;
  }
  else if ("dst-alpha" == name)
  {
    factor = EGEGraphics::BF_DST_ALPHA;
  }
  else if ("one-minus-src-alpha" == name)
  {
    factor = EGEGraphics::BF_ONE_MINUS_SRC_ALPHA;
  }
  else if ("one-minus-dst-alpha" == name)
  {
    factor = EGEGraphics::BF_ONE_MINUS_DST_ALPHA;
  }

  return factor;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(ResourceMaterial)
EGE_DEFINE_DELETE_OPERATORS(ResourceMaterial)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceMaterial::ResourceMaterial(Application* app, ResourceManager* manager) : IResource(app, manager, RESOURCE_NAME_MATERIAL), m_loaded(false)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceMaterial::~ResourceMaterial()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of resource. This method is a registration method for manager. */
PResource ResourceMaterial::Create(Application* app, ResourceManager* manager)
{
  return ege_new ResourceMaterial(app, manager);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Returns name of resource. */
const String& ResourceMaterial::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Initializes resource from XML. 
* 
*  \param  path  full path to resource definition file.
*  \param  tag   xml element with resource definition. 
*/
EGEResult ResourceMaterial::create(const String& path, const PXmlElement& tag)
{
  EGE_UNUSED(path);

  EGEResult result = EGE_SUCCESS;

  bool error = false;
  bool defaultPassInUse = false;

  PassData defaultPass;

  // get data
  m_name = tag->attribute("name");

  // also read data for default pass
  defaultPass.m_srcBlend        = MapBlendFactor(tag->attribute("src-blend").toLower(), EGEGraphics::BF_ONE);
  defaultPass.m_dstBlend        = MapBlendFactor(tag->attribute("dst-blend").toLower(), EGEGraphics::BF_ZERO);
  defaultPass.m_diffuseColor    = tag->attribute("diffuse-color", "1 1 1 1").toColor(&error);
  defaultPass.m_ambientColor    = tag->attribute("ambient-color", "1 1 1 1").toColor(&error);
  defaultPass.m_specularColor   = tag->attribute("specular-color", "0 0 0 1").toColor(&error);
  defaultPass.m_emissionColor   = tag->attribute("emission-color", "0 0 0 1").toColor(&error);
  defaultPass.m_shininess       = tag->attribute("shininess", "0").toFloat(&error);

  // check if obligatory data is wrong
  if (error || m_name.empty())
  {
    // error!
    EGE_PRINT("ResourceMaterial::create - failed for name: %s", m_name.toAscii());
    return EGE_ERROR_BAD_PARAM;
  }

  // go thru all sub node
  PXmlElement child = tag->firstChild();
  while (child->isValid())
  {
    // check child
    if (NODE_TEXTURE == child->name())
    {
      // textures without pass add to default one
      result = addTexture(child, defaultPass);
      
      // mark to indicate default pass is in use
      defaultPassInUse = true;
    }
    else if (NODE_PASS == child->name())
    {
      // add defined pass
      result = addPass(child);
    }

    // check if failed
    if (EGE_SUCCESS != result)
    {
      // error, done!
      break;
    }

    // go to next child
    child = child->nextChild();
  }

  // check if no single pass has been defined
  if (m_passes.empty() || defaultPassInUse)
  {
    // add default one to the front
    m_passes.insert(m_passes.begin(), defaultPass);
  }

  // apply override modifier to all passes
  for (PassDataArray::iterator it = m_passes.begin(); it != m_passes.end(); ++it)
  {
    PassData& pass = *it;

    if (tag->hasAttribute("src-blend"))
    {
      pass.m_srcBlend = defaultPass.m_srcBlend;
    }

    if (tag->hasAttribute("dst-blend"))
    {
      pass.m_dstBlend = defaultPass.m_dstBlend;
    }

    if (tag->hasAttribute("diffuse-color"))
    {
      pass.m_diffuseColor = defaultPass.m_diffuseColor;
    }

    if (tag->hasAttribute("ambient-color"))
    {
      pass.m_ambientColor = defaultPass.m_ambientColor;
    }

    if (tag->hasAttribute("specular-color"))
    {
      pass.m_specularColor = defaultPass.m_specularColor;
    }

    if (tag->hasAttribute("emission-color"))
    {
      pass.m_emissionColor = defaultPass.m_emissionColor;
    }

    if (tag->hasAttribute("shininess"))
    {
      pass.m_shininess = defaultPass.m_shininess;
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Loads resource. */
EGEResult ResourceMaterial::load()
{
  EGEResult result = EGE_SUCCESS;

  if (!isLoaded())
  {
    // load textures (for all passes)
    for (PassDataArray::iterator passIt = m_passes.begin(); passIt != m_passes.end(); ++passIt)
    {
      PassData& pass = *passIt;

      // load all textures for current pass
      for (TextureImageDataList::const_iterator it = pass.m_textureImageData.begin(); it != pass.m_textureImageData.end(); ++it)
      {
        const TextureImageData& textureImageData = *it;

        // check if manual texture
        if (textureImageData.m_manual)
        {
          // add placeholder only
          PTextureImage manual = ege_new TextureImage(app(), textureImageData.m_name);
          pass.m_textureImages.push_back(manual);
          continue;
        }

        PObject texture;

        // material referred texture space in use
        Rectf texRect(0, 0, 1, 1);
      
        // NOTE: Material can refer to Texture or TextureImage (ie. from atlas)

        // try to find TextureImage of a given name
        PResourceTextureImage textureImageRes = manager()->resource(RESOURCE_NAME_TEXTURE_IMAGE, textureImageData.m_name);
        if (textureImageRes)
        {
          // load texture image
          if (EGE_SUCCESS != (result = textureImageRes->load()))
          {
            // error!
            return result;
          }

          // retrieve referred texture
          TextureImage textureImage(app(), "");
          if (EGE_SUCCESS != (result = textureImageRes->setInstance(textureImage)))
          {
            // error!
            return result;
          }

          // store referred texture
          texture = textureImage.texture();

          // set new texture space in use
          texRect = textureImage.rect();
        }
        else
        {
          // try to find Texture of a given name
          PResourceTexture textureRes = manager()->resource(RESOURCE_NAME_TEXTURE, textureImageData.m_name);
          if (textureRes)
          {
            // load texture
            if (EGE_SUCCESS != (result = textureRes->load()))
            {
              // error!
              return result;
            }

            // retrieve referred texture
            texture = textureRes->texture();
          }
        }
      
        // check if not found
        if (NULL == texture)
        {
          // texture not found
          EGE_WARNING("Material texture not found: %s", textureImageData.m_name.toAscii());
          return EGE_ERROR;
        }

        // calculate final referred rectangle
        Rectf finalRect = texRect.combine(textureImageData.m_rect);

        PTextureImage textureImage = ege_new TextureImage(app(), texture, finalRect);
        if (NULL == textureImage || !textureImage->isValid())
        {
          // erro!
          return EGE_ERROR;
        }

        // set texture data
        textureImage->setEnvironmentMode(textureImageData.m_envMode);

        // add to pool
        pass.m_textureImages.push_back(textureImage);
      }
    }

    // set loaded
    m_loaded = true;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Unloads resource. */
void ResourceMaterial::unload()
{
  for (PassDataArray::iterator passIt = m_passes.begin(); passIt != m_passes.end(); ++passIt)
  {
    PassData& pass = *passIt;
    pass.m_textureImages.clear();
  }

  m_loaded = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds texture dependancy. */
EGEResult ResourceMaterial::addTexture(const PXmlElement& tag, PassData& pass)
{
  EGEResult result = EGE_SUCCESS;

  bool error = false;

  // get data
  String name                          = tag->attribute("name");
  Rectf rect                           = tag->attribute("rect", "0 0 1 1").toRectf(&error);
  EGETexture::EnvironmentMode envMode  = MapTextureEnvironmentMode(tag->attribute("env-mode", "modulate"), EGETexture::EM_MODULATE);
  bool manual                          = tag->attribute("manual", "false").toBool(&error);

  // check if obligatory data is wrong
  if (error || name.empty())
  {
    // error!
    EGE_PRINT("ResourceMaterial::addTexture - failed for name: %s", this->name().toAscii());
    return EGE_ERROR_BAD_PARAM;
  }

  // add into pool
  pass.m_textureImageData.push_back(TextureImageData(name, rect, envMode, manual));

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds pass. */
EGEResult ResourceMaterial::addPass(const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  // add new pass to pool
  m_passes.push_back(PassData());

  // retrieve reference to it
  PassData& pass = m_passes.back();

  bool error = false;

  // get data
  pass.m_srcBlend        = MapBlendFactor(tag->attribute("src-blend").toLower(), EGEGraphics::BF_ONE);
  pass.m_dstBlend        = MapBlendFactor(tag->attribute("dst-blend").toLower(), EGEGraphics::BF_ZERO);
  pass.m_diffuseColor    = tag->attribute("diffuse-color", "1 1 1 1").toColor(&error);
  pass.m_ambientColor    = tag->attribute("ambient-color", "1 1 1 1").toColor(&error);
  pass.m_specularColor   = tag->attribute("specular-color", "0 0 0 1").toColor(&error);
  pass.m_emissionColor   = tag->attribute("emission-color", "0 0 0 1").toColor(&error);
  pass.m_shininess       = tag->attribute("shininess", "0").toFloat(&error);

  if (error)
  {
    // error!
    return EGE_ERROR_BAD_PARAM;
  }

  // go thru all sub node
  PXmlElement child = tag->firstChild();
  while (child->isValid())
  {
    // check child
    if (NODE_TEXTURE == child->name())
    {
      result = addTexture(child, pass);
    }

    // check if failed
    if (EGE_SUCCESS != result)
    {
      // error, done!
      break;
    }

    // go to next child
    child = child->nextChild();
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of material object defined by resource. */
PMaterial ResourceMaterial::createInstance() const
{
	PMaterial object = ege_new Material(app());
  if (object)
  {
    // set new data
    if (EGE_SUCCESS != setInstance(object))
    {
      object = NULL;
    }
  }

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Set given instance of material object to what is defined by resource. */
EGEResult ResourceMaterial::setInstance(const PMaterial& instance) const
{
  // sanity check
  if (NULL == instance || !isLoaded())
  {
    return EGE_ERROR;
  }

  // clean up
  instance->clear();

  // create render passes
  for (PassDataArray::const_iterator it = m_passes.begin(); it != m_passes.end(); ++it)
  {
    const PassData& pass = *it;

    // add new pass
    RenderPass* renderPass = instance->addPass(NULL);
    if (NULL == renderPass)
    {
      // error!
      return EGE_ERROR;
    }

    // fill in data
    renderPass->setSrcBlendFactor(pass.m_srcBlend);
    renderPass->setDstBlendFactor(pass.m_dstBlend);

    renderPass->setAmbientColor(pass.m_ambientColor);
    renderPass->setDiffuseColor(pass.m_diffuseColor);
    renderPass->setSpecularColor(pass.m_specularColor);
    renderPass->setEmissionColor(pass.m_emissionColor);

    renderPass->setShininess(pass.m_shininess);

    renderPass->removeTexture(-1);
    for (TextureImageList::const_iterator it = pass.m_textureImages.begin(); it != pass.m_textureImages.end(); ++it)
    {
      EGEResult result;
      if (EGE_SUCCESS != (result = renderPass->addTexture(*it)))
      {
        // error!
        return result;
      }
    }
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns source pixel factor function for a given pass. */
EGEGraphics::BlendFactor ResourceMaterial::srcBlendFactor(u32 pass) const 
{ 
  return (pass < passCount()) ? m_passes[pass].m_srcBlend : EGEGraphics::BF_ONE; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns destination pixel factor function for a given pass. */
EGEGraphics::BlendFactor ResourceMaterial::dstBlendFactor(u32 pass) const 
{ 
  return (pass < passCount()) ? m_passes[pass].m_dstBlend : EGEGraphics::BF_ZERO; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns ambient color for a given pass. */
const Color& ResourceMaterial::ambientColor(u32 pass) const
{ 
  return (pass < passCount()) ? m_passes[pass].m_ambientColor : Color::WHITE; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns diffuse color for a given pass. */
const Color& ResourceMaterial::diffuseColor(u32 pass) const 
{ 
  return (pass < passCount()) ? m_passes[pass].m_diffuseColor : Color::WHITE; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns specular color. */
const Color& ResourceMaterial::specularColor(u32 pass) const 
{
  return (pass < passCount()) ? m_passes[pass].m_specularColor : Color::BLACK; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns emission color for a given pass. */
const Color& ResourceMaterial::emissionColor(u32 pass) const
{ 
  return (pass < passCount()) ? m_passes[pass].m_emissionColor : Color::BLACK; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns shinness value for a given pass. */
float32 ResourceMaterial::shininess(u32 pass) const 
{ 
  return (pass < passCount()) ? m_passes[pass].m_shininess : 0.0f; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
