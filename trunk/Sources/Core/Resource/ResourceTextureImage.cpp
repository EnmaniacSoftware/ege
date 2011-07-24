#include "Core/Resource/ResourceTextureImage.h"
#include "Core/Resource/ResourceTexture.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Graphics/TextureImage.h"
#include <EGEXml.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(ResourceTextureImage)
EGE_DEFINE_DELETE_OPERATORS(ResourceTextureImage)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceTextureImage::ResourceTextureImage(Application* app, ResourceManager* manager) : IResource(app, manager, "texture-image")
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

  // get data
  m_name        = tag->attribute("name");
  m_textureName = tag->attribute("texture");
  m_rect        = tag->attribute("rect", "0 0 0 0");

  // check if obligatory data is wrong
  if (m_name.empty() || m_textureName.empty())
  {
    // error!
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
    PResourceTexture texture = manager()->resource("texture", textureName());
    if (texture)
    {
      // load texture
      if (EGE_SUCCESS != (result = texture->load()))
      {
        // error!
        return result;
      }

      // create texture image object
      m_textureImage = ege_new TextureImage(app(), texture->texture(), rect().toRectf());
      if (NULL == m_textureImage || !m_textureImage->isValid())
      {
        // error!
        return EGE_ERROR;
      }
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Unloads resource. */
void ResourceTextureImage::unload() 
{ 
  m_textureImage = NULL; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
