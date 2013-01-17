#include "Core/Resource/ResourceTexture.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Graphics/Graphics.h"
#include "Core/Graphics/HardwareResourceProvider.h"
#include "Core/Graphics/Render/RenderSystem.h"
#include <EGEApplication.h>
#include <EGEGraphics.h>
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
ResourceTexture::ResourceTexture(Application* app, ResourceGroup* group) : IResource(app, group, RESOURCE_NAME_TEXTURE),
                                                                           m_resourceRequestId(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceTexture::~ResourceTexture()
{
  if ((NULL != app()->graphics()) && (NULL != app()->graphics()->hardwareResourceProvider()))
  {
    ege_disconnect(app()->graphics()->hardwareResourceProvider(), requestComplete, this, ResourceTexture::onRequestComplete);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PResource ResourceTexture::Create(Application* app, ResourceGroup* group)
{
  return ege_new ResourceTexture(app, group);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PResource ResourceTexture::Create(Application* app, ResourceGroup* group, const String& name, PObject texture)
{
  // create empty resource
  PResourceTexture resource = Create(app, group);
  if (resource)
  {
    resource->m_name        = name;
    resource->m_texture     = texture;
    resource->m_manual      = true;
  }

  return resource;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& ResourceTexture::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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

  // check if obligatory data is wrong
  if (m_name.empty() || m_path.empty() || m_type.empty() || error)
  {
    // error!
    egeWarning() << "Failed for name:" << m_name;
    return EGE_ERROR_BAD_PARAM;
  }

  // compose absolute path
  m_path = path + "/" + m_path;

  // set state
  m_state = STATE_UNLOADED;

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceTexture::load()
{
  EGEResult result = EGE_SUCCESS;

  if (STATE_UNLOADED == m_state)
  {
    // create according to type
    if ("2d" == type())
    {
      result = create2D();
      if (EGE_SUCCESS == result)
      {
        // set to loading
        m_state = STATE_LOADING;
      }
    }
  }
  
  // check if still loading
  if (STATE_LOADING == m_state)
  {
    result = EGE_WAIT;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceTexture::create2D()
{
  // load image data
  PImage image = Image::Load(path());
  if (NULL == image)
  {
    // error!
    return EGE_ERROR;
  }

  // apply texture filters
  app()->graphics()->renderSystem()->setTextureMinFilter(minFilter());
  app()->graphics()->renderSystem()->setTextureMagFilter(magFilter());
  app()->graphics()->renderSystem()->setTextureAddressingModeS(adressingModeS());
  app()->graphics()->renderSystem()->setTextureAddressingModeT(adressingModeT());

  // request texture
  m_resourceRequestId = app()->graphics()->hardwareResourceProvider()->requestCreateTexture2D(name(), image);

  // connect for notification
  ege_connect(app()->graphics()->hardwareResourceProvider(), requestComplete, this, ResourceTexture::onRequestComplete);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceTexture::unload() 
{ 
  if (STATE_LOADED == m_state)
  {
    if ("2d" == type())
    {
      app()->graphics()->hardwareResourceProvider()->requestDestroyTexture2D(m_texture);

      // clean up
      m_texture = NULL; 
    }

    // reset flag
    m_state = STATE_UNLOADED;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceTexture::onRequestComplete(u32 handle, PObject object)
{
  if (handle == m_resourceRequestId)
  {
    // store handle
    m_texture = object;

    // disconnect
    ege_disconnect(app()->graphics()->hardwareResourceProvider(), requestComplete, this, ResourceTexture::onRequestComplete);
    m_resourceRequestId = 0;

    // set state
    m_state = STATE_LOADED;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END