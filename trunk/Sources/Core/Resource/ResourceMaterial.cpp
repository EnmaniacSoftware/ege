#include "Core/Resource/ResourceMaterial.h"
#include "Core/Resource/ResourceTexture.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Graphics/Material.h"
#include "Core/Graphics/TextureImage.h"
#include <EGETexture.h>

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

EGE_DEFINE_NEW_OPERATORS(ResourceMaterial)
EGE_DEFINE_DELETE_OPERATORS(ResourceMaterial)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceMaterial::ResourceMaterial(Application* app, ResourceManager* manager) : IResource(app, manager, "material")
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

  // get data
  m_name            = tag->attribute("name");
  m_srcBlend        = mapBlendFactor(tag->attribute("src-blend").toLower(), EGEGraphics::BLEND_FACTOR_ONE);
  m_dstBlend        = mapBlendFactor(tag->attribute("dst-blend").toLower(), EGEGraphics::BLEND_FACTOR_ZERO);
  m_diffuseColor    = tag->attribute("diffuse-color");
  m_ambientColor    = tag->attribute("ambient-color");
  m_specularColor   = tag->attribute("specular-color");
  m_emissionColor   = tag->attribute("emission-color");
  m_shinness        = tag->attribute("shinness");

  // check if obligatory data is wrong
  if (m_name.empty() || (EGEGraphics::BLEND_FACTOR_UNKNOWN == m_srcBlend) || (EGEGraphics::BLEND_FACTOR_UNKNOWN == m_dstBlend))
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
    bool error = false;

    PMaterial material = ege_new Material(app());
    if (NULL == material)
    {
      // error!
      return EGE_ERROR_NO_MEMORY;
    }

    // set blending parameters
    material->setSrcBlendFactor(srcBlendFactor());
    material->setDstBlendFactor(dstBlendFactor());

    // set colors
    if (!ambientColorName().empty())
    {
      material->setAmbientColor(ambientColorName().toColor(&error));
    }

    if (!diffuseColorName().empty())
    {
      material->setDiffuseColor(diffuseColorName().toColor(&error));
    }

    if (!specularColorName().empty())
    {
      material->setSpecularColor(specularColorName().toColor(&error));
    }

    if (!emissionColorName().empty())
    {
      material->setEmissionColor(emissionColorName().toColor(&error));
    }

    if (!shinnessName().empty())
    {
      material->setShinness(shinnessName().toFloat(&error));
    }

    // check for errors
    if (error)
    {
      return EGE_ERROR;
    }

    // load textures
    for (List<TextureImageData>::const_iterator it = m_textureImages.begin(); it != m_textureImages.end(); ++it)
    {
      const TextureImageData& textureImageData = *it;

      PResourceTexture texture = manager()->resource("texture", textureImageData.m_name);
      if (texture)
      {
        // load texture
        if (EGE_SUCCESS != (result = texture->load()))
        {
          // error!
          return result;
        }

        // NOTE: assumption textureImageData data are valid
        PTextureImage textureImage = ege_new TextureImage(app(), texture->texture(), textureImageData.m_rect.toRectf());
        if (NULL == textureImage || !textureImage->isValid())
        {
          // erro!
          return EGE_ERROR;
        }

        // set texture data
        textureImage->setEnvironmentMode(MapTextureEnvironmentMode(textureImageData.m_envMode, EGETexture::EM_MODULATE));

        // add texture to material
        if (EGE_SUCCESS != (result = material->addTexture(textureImage)))
        {
          // error!
          return result;
        }
      }
      else
      {
        // texture not found
        EGE_PRINT(String::Format("Texture not found: %s", textureImageData.m_name.toAscii()));
        return EGE_ERROR;
      }
    }

    // success
    m_material = material;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Unloads resource. */
void ResourceMaterial::unload()
{
  m_material = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds texture dependancy. */
EGEResult ResourceMaterial::addTexture(const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  // get data
  String name     = tag->attribute("name");
  String rect     = tag->attribute("rect", "0 0 1 1");
  String envMode  = tag->attribute("env-mode", "modulate");

  // check if obligatory data is wrong
  if (name.empty())
  {
    // error!
    return EGE_ERROR_BAD_PARAM;
  }

  // add into pool
  m_textureImages.push_back(TextureImageData(name, rect, envMode));

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Maps blend factor's name into value. */
EGEGraphics::EBlendFactor ResourceMaterial::mapBlendFactor(const String& name, EGEGraphics::EBlendFactor defaultValue) const
{
  // check if no data to convert
  if (name.empty())
  {
    // return default value
    return defaultValue;
  }

  // map value
  EGEGraphics::EBlendFactor factor = EGEGraphics::BLEND_FACTOR_UNKNOWN;

  if ("zero" == name)
  {
    factor = EGEGraphics::BLEND_FACTOR_ZERO;
  }
  else if ("one" == name)
  {
    factor = EGEGraphics::BLEND_FACTOR_ONE;
  }
  else if ("src-color" == name)
  {
    factor = EGEGraphics::BLEND_FACTOR_SRC_COLOR;
  }
  else if ("dst-color" == name)
  {
    factor = EGEGraphics::BLEND_FACTOR_DST_COLOR;
  }
  else if ("one-minus-src-color" == name)
  {
    factor = EGEGraphics::BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
  }
  else if ("one-minus-dst-color" == name)
  {
    factor = EGEGraphics::BLEND_FACTOR_ONE_MINUS_DST_COLOR;
  }
  else if ("src-alpha" == name)
  {
    factor = EGEGraphics::BLEND_FACTOR_SRC_ALPHA;
  }
  else if ("dst-alpha" == name)
  {
    factor = EGEGraphics::BLEND_FACTOR_DST_ALPHA;
  }
  else if ("one-minus-src-alpha" == name)
  {
    factor = EGEGraphics::BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
  }
  else if ("one-minus-dst-alpha" == name)
  {
    factor = EGEGraphics::BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
  }

  return factor;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

