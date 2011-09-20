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

#define NODE_TEXTURE "texture"

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

  // get data
  m_name            = tag->attribute("name");
  m_srcBlend        = MapBlendFactor(tag->attribute("src-blend").toLower(), EGEGraphics::BF_ONE);
  m_dstBlend        = MapBlendFactor(tag->attribute("dst-blend").toLower(), EGEGraphics::BF_ZERO);
  m_diffuseColor    = tag->attribute("diffuse-color", "1 1 1 1").toColor(&error);
  m_ambientColor    = tag->attribute("ambient-color", "1 1 1 1").toColor(&error);
  m_specularColor   = tag->attribute("specular-color", "0 0 0 1").toColor(&error);
  m_emissionColor   = tag->attribute("emission-color", "0 0 0 1").toColor(&error);
  m_shininess       = tag->attribute("shininess", "0").toFloat(&error);

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
      result = addTexture(child);
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
/*! IResource override. Loads resource. */
EGEResult ResourceMaterial::load()
{
  EGEResult result = EGE_SUCCESS;

  if (!isLoaded())
  {
    // load textures
    for (TextureImageDataList::const_iterator it = m_textureImageData.begin(); it != m_textureImageData.end(); ++it)
    {
      const TextureImageData& textureImageData = *it;

      // check if manual texture
      if (textureImageData.m_manual)
      {
        // add placeholder only
        PTextureImage manual = ege_new TextureImage(app(), textureImageData.m_name);
        m_textureImages.push_back(manual);
        continue;
      }

      PObject texture;

      // material referred texture space in use
      Rectf texRect(0, 0, 1, 1);
      
      // NOTE: Material can refer to Texture or TextureImage (ie. from atlas)

      // try to find TextureImage of a given name
      PResourceTextureImage textureImageRes = manager()->resource("texture-image", textureImageData.m_name);
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
        PResourceTexture textureRes = manager()->resource("texture", textureImageData.m_name);
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
      m_textureImages.push_back(textureImage);
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
  m_textureImages.clear();

  m_loaded = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds texture dependancy. */
EGEResult ResourceMaterial::addTexture(const PXmlElement& tag)
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
  m_textureImageData.push_back(TextureImageData(name, rect, envMode, manual));

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

  // fill in data
  instance->setSrcBlendFactor(srcBlendFactor());
  instance->setDstBlendFactor(dstBlendFactor());

  instance->setAmbientColor(ambientColor());
  instance->setDiffuseColor(diffuseColor());
  instance->setSpecularColor(specularColor());
  instance->setEmissionColor(emissionColor());

  instance->setShininess(shininess());

  instance->removeTexture(-1);
  for (TextureImageList::const_iterator it = m_textureImages.begin(); it != m_textureImages.end(); ++it)
  {
    EGEResult result;
    if (EGE_SUCCESS != (result = instance->addTexture(*it)))
    {
      // error!
      return result;
    }
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
