#include "Core/Resource/ResourceSpritesheet.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/ResourceTextureImage.h"
#include <EGEXml.h>
#include <EGEDebug.h>
#include <EGEResources.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(ResourceSpritesheet)
EGE_DEFINE_DELETE_OPERATORS(ResourceSpritesheet)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceSpritesheet::ResourceSpritesheet(Application* app, ResourceManager* manager) : IResource(app, manager, RESOURCE_NAME_SPRITE_SHEET)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceSpritesheet::~ResourceSpritesheet()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of resource. This method is a registration method for manager. */
PResource ResourceSpritesheet::Create(Application* app, ResourceManager* manager)
{
  return ege_new ResourceSpritesheet(app, manager);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Returns name of resource. */
const String& ResourceSpritesheet::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Initializes resource from XML. 
* 
*  \param  path  full path to resource definition file.
*  \param  tag   xml element with resource definition. 
*/
EGEResult ResourceSpritesheet::create(const String& path, const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  bool error = false;

  // get data
  m_name            = tag->attribute("name");
  m_textureName     = tag->attribute("texture");
  m_frameWidth      = tag->attribute("frame-width").toInt(&error);
  m_frameHeight     = tag->attribute("frame-height").toInt(&error);
  m_frameCount      = tag->attribute("frames").toInt(&error);
  m_framesPerRow    = tag->attribute("frames-per-row").toInt(&error);

  // check if obligatory data is wrong
  if (error || m_name.empty() || m_textureName.empty())
  {
    // error!
    EGE_PRINT("ResourceSpritesheet::create - failed for name: %s", m_name.toAscii());
    return EGE_ERROR_BAD_PARAM;
  }

  // compose absolute path
 // m_path = path + "/" + m_path;

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Loads resource. */
EGEResult ResourceSpritesheet::load()
{
  EGEResult result = EGE_SUCCESS;

  if (!isLoaded())
  {
    // get texture
    PResourceTextureImage textureImageResource = manager()->resource("texture-image", textureName());
    if (textureImageResource)
    {
      // load texture
      if (EGE_SUCCESS != (result = textureImageResource->load()))
      {
        // error!
        return result;
      }

      // store texture
      m_texture = textureImageResource;
    }
    else
    {
      // material not found
      result = EGE_ERROR_NOT_FOUND;
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Unloads resource. */
void ResourceSpritesheet::unload() 
{ 
  EGE_PRINT("ResourceSpritesheet::unload - %s", name().toAscii());

  // unload texture
  m_texture = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
