#include "Core/Resource/ResourceTextureImage.h"
#include "Core/Resource/ResourceTexture.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Graphics/TextureImage.h"
#include <EGEXml.h>
#include <EGEDebug.h>
#include <EGEResources.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(ResourceTextureImage)
EGE_DEFINE_DELETE_OPERATORS(ResourceTextureImage)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceTextureImage::ResourceTextureImage(Application* app, ResourceManager* manager) : IResource(app, manager, RESOURCE_NAME_TEXTURE_IMAGE)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceTextureImage::~ResourceTextureImage()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Creates instance of resource. This method is a registration method for manager. */
PResource ResourceTextureImage::Create(Application* app, ResourceManager* manager)
{
  return ege_new ResourceTextureImage(app, manager);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Returns name of resource. */
const String& ResourceTextureImage::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Initializes resource from XML. 
* 
*  \param  path  full path to resource definition file.
*  \param  tag   xml element with resource definition. 
*/
EGEResult ResourceTextureImage::create(const String& path, const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  bool error = false;

  // get data
  m_name        = tag->attribute("name");
  m_textureName = tag->attribute("texture");
  m_rect        = tag->attribute("rect", "0 0 0 0").toRectf(&error);

  // check if obligatory data is wrong
  if (error || m_name.empty() || m_textureName.empty())
  {
    // error!
    EGE_PRINT("ResourceTextureImage::create - failed for name: %s", m_name.toAscii());
    return EGE_ERROR_BAD_PARAM;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Loads resource. */
EGEResult ResourceTextureImage::load()
{
  EGEResult result = EGE_SUCCESS;

  if (!isLoaded())
  {
    // load texture
    PResourceTexture textureResource = manager()->resource("texture", m_textureName);
    if (textureResource)
    {
      // load texture
      if (EGE_SUCCESS != (result = textureResource->load()))
      {
        // error!
        return result;
      }

      // store texture object
      m_texture = textureResource->texture();
    }
    else
    {
      EGE_WARNING("ResourceTextureImage::load - Could not find texture resource - %s", m_textureName.toAscii());
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Unloads resource. */
void ResourceTextureImage::unload() 
{ 
  m_texture = NULL; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns instance of texture image object defined by resource. 
 * @note  Loads resource if it is not loaded yet.
 */
PTextureImage ResourceTextureImage::createInstance()
{
	PTextureImage object = ege_new TextureImage(app(), name());
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
/*! Set given instance of texture image object to what is defined by resource. 
 * @note  Loads resource if it is not loaded yet.
 */
EGEResult ResourceTextureImage::setInstance(const PTextureImage& instance)
{
  // sanity check
  if (NULL == instance)
  {
    // error!
    return EGE_ERROR;
  }

  // check if not loaded yet
  if (!isLoaded())
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
  
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
