#include "Core/Resource/ResourceMaterial.h"
#include "Core/Resource/ResourceTexture.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Graphics/Material.h"
#include <EGETexture.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#define NODE_TEXTURE "texture"

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
const EGEString& ResourceMaterial::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Initializes resource from XML. 
* 
*  \param  path  full path to resource definition file.
*  \param  tag   xml element with resource definition. 
*/
EGEResult ResourceMaterial::create(const EGEString& path, const PXmlElement& tag)
{
  EGE_UNUSED(path);

  EGEResult result = EGE_SUCCESS;

  // get data
  m_name            = tag->attribute("name");
  m_srcBlend        = tag->attribute("src-blend").toLower();
  m_dstBlend        = tag->attribute("dst-blend").toLower();
  m_blend           = tag->attribute("blend");
  m_diffuseColor    = tag->attribute("diffuse-color");
  m_ambientColor    = tag->attribute("ambient-color");
  m_specularColor   = tag->attribute("specular-color");
  m_emissionColor   = tag->attribute("emission-color");
  m_shinness        = tag->attribute("shinness");

  // check if obligatory data is wrong
  if (m_name.empty())
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
    if (!srcBlendFuncName().empty())
    {
      material->setSrcBlendFunc(srcBlendFuncName());
    }

    if (!dstBlendFuncName().empty())
    {
      material->setDstBlendFunc(dstBlendFuncName());
    }

    if (!blendingEnabledName().empty())
    {
      material->enableBlending(blendingEnabledName().toBool(&error));
    }

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
    for (EGEStringList::const_iterator it = m_textureNames.begin(); it != m_textureNames.end(); ++it)
    {
      PResourceTexture texture = manager()->resource("texture", *it);
      if (texture)
      {
        // load texture
        if (EGE_SUCCESS != (result = texture->load()))
        {
          // error!
          return result;
        }

        // add texture to material
        if (EGE_SUCCESS != (result = material->addTexture(texture->texture())))
        {
          // error!
          return result;
        }
      }
      else
      {
        // texture not found
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
  EGEString name = tag->attribute("name");

  // check if obligatory data is wrong
  if (name.empty())
  {
    // error!
    return EGE_ERROR_BAD_PARAM;
  }

  // add texture name
  m_textureNames.push_back(name);

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

