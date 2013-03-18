#include "Core/Resource/ResourceSpritesheet.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/ResourceTextureImage.h"
#include "EGEXml.h"
#include "EGEDebug.h"
#include "EGEResources.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceSpritesheet)
EGE_DEFINE_DELETE_OPERATORS(ResourceSpritesheet)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceSpritesheet::ResourceSpritesheet(Application* app, ResourceGroup* group) : IResource(app, group, RESOURCE_NAME_SPRITE_SHEET)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceSpritesheet::~ResourceSpritesheet()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PResource ResourceSpritesheet::Create(Application* app, ResourceGroup* group)
{
  return ege_new ResourceSpritesheet(app, group);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& ResourceSpritesheet::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceSpritesheet::create(const String& path, const PXmlElement& tag)
{
  EGE_UNUSED(path);

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
    egeWarning() << "Failed for name:" << m_name;
    return EGE_ERROR_BAD_PARAM;
  }

  // set state
  m_state = STATE_UNLOADED;

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceSpritesheet::load()
{
  EGEResult result = EGE_SUCCESS;

  if (STATE_LOADED != m_state)
  {
    // get texture image...
    PResourceTextureImage textureImageResource = group()->manager()->resource(RESOURCE_NAME_TEXTURE_IMAGE, textureName());
    if (textureImageResource)
    {
      // load texture
      if (EGE_SUCCESS != (result = textureImageResource->load()))
      {
        // error!
        return result;
      }

      // store texture
      m_texture = textureImageResource->createInstance();
    }
    else
    {
      // try texture...
      PResourceTexture textureResource = group()->manager()->resource(RESOURCE_NAME_TEXTURE, textureName());
      if (NULL != textureResource)
      {
        // load texture
        if (EGE_SUCCESS != (result = textureResource->load()))
        {
          // error!
          return result;
        }

        // store texture
        m_texture = ege_new TextureImage(app(), textureResource->texture(), Rectf(0, 0, 1.0f, 1.0f));
      }
      else
      {
        // material not found
        result = EGE_ERROR_NOT_FOUND;
      }
    }

    // check if successful
    if (EGE_SUCCESS == result)
    {
      // set flag
      m_state = STATE_LOADED;
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceSpritesheet::unload() 
{ 
  // unload texture
  m_texture = NULL;

  // reset flag
  m_state = STATE_UNLOADED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END