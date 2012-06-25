#include "Core/Resource/ResourceTexture.h"
#include "Core/Resource/ResourceManager.h"
#include <EGEXml.h>
#include <EGEDebug.h>
#include <EGEResources.h>

#if EGE_RENDERING_OPENGL_2 || EGE_RENDERING_OPENGLES_1
#include "Core/Graphics/OpenGL/Texture2DOGL.h"
#endif // EGE_RENDERING_OPENGL_2 || EGE_RENDERING_OPENGLES_1

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceTexture)
EGE_DEFINE_DELETE_OPERATORS(ResourceTexture)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function mapping literal filter function name into numeric value. */
static EGETexture::Filter MapFilterName(const String& name, EGETexture::Filter defaultValue)
{
  if ("trilinear" == name)
  {
    return EGETexture::TRILINEAR;
  }
  else if ("mipmap-bilinear" == name)
  {
    return EGETexture::MIPMAP_BILINEAR;
  }
  else if ("mipmap-trilinear" == name)
  {
    return EGETexture::MIPMAP_TRILINEAR;
  }

  return defaultValue;//EGETexture::BILINEAR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function mapping literal texture addressing mode name into numeric value. */
static EGETexture::AddressingMode MapTextureAddressingName(const String& name, EGETexture::AddressingMode defaultValue)
{
  if ("clamp" == name)
  {
    return EGETexture::AM_CLAMP;
  }
  else if ("repeat" == name)
  {
    return EGETexture::AM_REPEAT;
  }

  return defaultValue; //EGETexture::AM_REPEAT;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceTexture::ResourceTexture(Application* app, ResourceManager* manager) : IResource(app, manager, RESOURCE_NAME_TEXTURE),
                                                                               m_compression(false)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceTexture::~ResourceTexture()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of resource. This method is a registration method for manager. */
PResource ResourceTexture::Create(Application* app, ResourceManager* manager)
{
  return ege_new ResourceTexture(app, manager);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of resource embedding given texture object. This is helper method for manual texture adding. */
PResource ResourceTexture::Create(Application* app, ResourceManager* manager, const String& name, PObject texture)
{
  // create empty resource
  PResourceTexture resource = Create(app, manager);
  if (resource)
  {
    resource->m_name        = name;
    resource->m_texture     = texture;
    resource->m_manual      = true;
  }

  return resource;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Returns name of resource. */
const String& ResourceTexture::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Initializes resource from XML. 
* 
*  \param  path  full path to resource definition file.
*  \param  tag   xml element with resource definition. 
*/
EGEResult ResourceTexture::create(const String& path, const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  bool error = false;

  // get data
  m_name            = tag->attribute("name");
  m_path            = tag->attribute("path");
  m_type            = tag->attribute("type").toLower();
  m_minFilter       = MapFilterName(tag->attribute("min-filter").toLower(), EGETexture::BILINEAR);
  m_magFilter       = MapFilterName(tag->attribute("mag-filter").toLower(), EGETexture::BILINEAR);
  m_addressingModeS = MapTextureAddressingName(tag->attribute("mode-s").toLower(), EGETexture::AM_REPEAT);
  m_addressingModeT = MapTextureAddressingName(tag->attribute("mode-t").toLower(), EGETexture::AM_REPEAT);
  m_rotation        = tag->attribute("rotation", "0").toAngle(&error);
  m_compression     = tag->attribute("compress", "false").toBool(&error);

  // check if obligatory data is wrong
  if (m_name.empty() || m_path.empty() || m_type.empty() || error)
  {
    // error!
    EGE_PRINT("ERROR: Failed for name: %s", m_name.toAscii());
    return EGE_ERROR_BAD_PARAM;
  }

  // compose absolute path
  m_path = path + "/" + m_path;

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Loads resource. */
EGEResult ResourceTexture::load()
{
  EGEResult result = EGE_SUCCESS;

  if (!isLoaded())
  {
    // create according to type
    if ("2d" == type())
    {
  //    EGE_PRINT("%s, 2D", name().toAscii());
      result = create2D();
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates 2D texture. */
EGEResult ResourceTexture::create2D()
{
  EGEResult result = EGE_SUCCESS;

  PTexture2D texture = ege_new Texture2D(app(), name());
  if ((NULL == texture) || !texture->isValid())
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // sets parameters
  texture->setMinFilter(minFilter());
  texture->setMagFilter(magFilter());
  texture->setTextureAddressingModeS(adressingModeS());
  texture->setTextureAddressingModeT(adressingModeT());
  texture->setCompressionEnabled(m_compression);

  // create it
  result = texture->create(path());
  
  // assign
  m_texture = texture;

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Unloads resource. */
void ResourceTexture::unload() 
{ 
  EGE_PRINT("%s", name().toAscii());

  m_texture = NULL; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Returns TRUE if texture is loaded. */
bool ResourceTexture::isLoaded() const 
{ 
  return NULL != m_texture; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END