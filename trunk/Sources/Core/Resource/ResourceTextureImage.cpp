#include "Core/Resource/ResourceTextureImage.h"
#include "Core/Resource/ResourceTexture.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Graphics/TextureImage.h"
#include <EGEXml.h>
#include <EGEDebug.h>
#include <EGEResources.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceTextureImage)
EGE_DEFINE_DELETE_OPERATORS(ResourceTextureImage)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceTextureImage::ResourceTextureImage(Application* app, ResourceGroup* group) : IResource(app, group, RESOURCE_NAME_TEXTURE_IMAGE)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceTextureImage::~ResourceTextureImage()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PResource ResourceTextureImage::Create(Application* app, ResourceGroup* group)
{
  return ege_new ResourceTextureImage(app, group);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& ResourceTextureImage::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceTextureImage::create(const String& path, const PXmlElement& tag)
{
  EGE_UNUSED(path);

  EGEResult result = EGE_SUCCESS;

  bool error = false;

  // get data
  m_name          = tag->attribute("name");
  m_textureName   = tag->attribute("texture");
  m_rect          = tag->attribute("rect", "0 0 0 0").toRectf(&error);
  m_rotationAngle = tag->attribute("rotation", "0").toAngle(&error);

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
EGEResult ResourceTextureImage::load()
{
  EGEResult result = EGE_SUCCESS;

  if (STATE_LOADED != m_state)
  {
    // load texture
    PResourceTexture textureResource = group()->manager()->resource(RESOURCE_NAME_TEXTURE, m_textureName);
    if (NULL != textureResource)
    {
      // load texture
      if (EGE_SUCCESS != (result = textureResource->load()))
      {
        // error!
        return result;
      }

      // store texture object
      m_texture = textureResource->texture();

      // set flag
      m_state = STATE_LOADED;
    }
    else
    {
      egeWarning() << "ResourceTextureImage::load - Could not find texture resource - " << m_textureName;
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceTextureImage::unload() 
{ 
  // clean up
  m_texture = NULL; 

  // reset flag
  m_state = STATE_UNLOADED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PTextureImage ResourceTextureImage::createInstance()
{
	PTextureImage object = ege_new TextureImage(app());
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
EGEResult ResourceTextureImage::setInstance(const PTextureImage& instance)
{
  // sanity check
  if (NULL == instance)
  {
    // error!
    return EGE_ERROR;
  }

  // check if not loaded yet
  // TAGE - setInstance should fail if not loaded, check it out and correct
  if (STATE_LOADED != m_state)
  {
    // load
    EGEResult result;
    if (EGE_SUCCESS != (result = load()))
    {
      // error!
      return result;
    }
  }

  // fill in data
  instance->setTexture(m_texture);
  instance->setRect(m_rect);
  instance->setName(name());
  instance->setRotationAngle(m_rotationAngle);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END