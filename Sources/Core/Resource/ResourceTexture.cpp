#include "Core/Resource/ResourceTexture.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Graphics/Graphics.h"
#include "Core/Graphics/HardwareResourceProvider.h"
#include "Core/Graphics/Render/RenderSystem.h"
#include "EGEApplication.h"
#include "EGEGraphics.h"
#include "EGEXml.h"
#include "EGEResources.h"
#include "EGEStringUtils.h"
#include "EGEDirectory.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KResourceTextureDebugName = "EGEResourceTexture";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceTexture)
EGE_DEFINE_DELETE_OPERATORS(ResourceTexture)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function mapping literal filter function name into numeric value. */
static TextureFilter MapFilterName(const String& name, TextureFilter defaultValue)
{
  if ("trilinear" == name)
  {
    return TF_TRILINEAR;
  }
  else if ("bilinear" == name)
  {
    return TF_BILINEAR;
  }
  else if ("none" == name)
  {
    return TF_NEAREST;
  }

  return defaultValue;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function mapping literal texture addressing mode name into numeric value. */
static TextureAddressingMode MapTextureAddressingName(const String& name, TextureAddressingMode defaultValue)
{
  if ("clamp" == name)
  {
    return AM_CLAMP;
  }
  else if ("repeat" == name)
  {
    return AM_REPEAT;
  }

  return defaultValue; //EGETexture::AM_REPEAT;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceTexture::ResourceTexture(Application* app, ResourceGroup* group) : IResource(app, group, RESOURCE_NAME_TEXTURE),
                                                                           m_minFilter(TF_NEAREST),
                                                                           m_magFilter(TF_NEAREST),
                                                                           m_addressingModeS(AM_REPEAT),
                                                                           m_addressingModeT(AM_REPEAT),
                                                                           m_resourceRequestId(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceTexture::~ResourceTexture()
{
//  if (name() == "main-menu-heart")
//  {
//    int a = 1;
//  }

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
    resource->m_name    = name;
    resource->m_texture = texture;
    resource->m_manual  = true;
    resource->m_state   = STATE_LOADED;

    if (EGE_OBJECT_UID_TEXTURE_2D == texture->uid())
    {
      resource->m_type = "2d";
    }
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
  m_dataType        = tag->attribute("data");
  m_type            = tag->attribute("type").toLower();
  m_minFilter       = MapFilterName(tag->attribute("min-filter").toLower(), TF_NEAREST);
  m_magFilter       = MapFilterName(tag->attribute("mag-filter").toLower(), TF_NEAREST);
  m_addressingModeS = MapTextureAddressingName(tag->attribute("mode-s").toLower(), AM_REPEAT);
  m_addressingModeT = MapTextureAddressingName(tag->attribute("mode-t").toLower(), AM_REPEAT);
  m_rotation        = StringUtils::ToAngle(tag->attribute("rotation", "0"), &error);

  // check if any embedded data type
  if ( ! m_dataType.empty())
  {
    // load texture data from XML
    if (EGE_SUCCESS != (result = loadTextureData(tag)))
    {
      // error!
      return result;
    }
  }

  // check if obligatory data is wrong
  if (m_name.empty() || m_type.empty() || error || (m_path.empty() && m_dataType.empty()))
  {
    // error!
    egeWarning(KResourceTextureDebugName) << "Failed for name:" << m_name;
    return EGE_ERROR_BAD_PARAM;
  }

  // compose absolute path
  m_path = Directory::Join(path, m_path);

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
  PImage image = (NULL != m_data) ? Image::Load(m_data) : Image::Load(path());
  if (NULL == image)
  {
    // error!
    return EGE_ERROR;
  }

  //if (path().endsWith("grid.jpg"))
  //{
  //  Image::Save(image, "ala.png");
  //}

  // apply texture filters
  app()->graphics()->renderSystem()->setTextureMinFilter(minFilter());
  app()->graphics()->renderSystem()->setTextureMagFilter(magFilter());
  app()->graphics()->renderSystem()->setTextureAddressingModeS(adressingModeS());
  app()->graphics()->renderSystem()->setTextureAddressingModeT(adressingModeT());

  // request texture
  m_resourceRequestId = app()->graphics()->hardwareResourceProvider()->requestCreateTexture2D(name(), image);

  // connect for notification
  egeDebug(KResourceTextureDebugName) << "Awaiting" << group()->name() << "for" << m_resourceRequestId;
  ege_connect(app()->graphics()->hardwareResourceProvider(), requestComplete, this, ResourceTexture::onRequestComplete);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceTexture::unload() 
{ 
  //if (group()->name() == "main-menu-screen")
  //{
  //  int a = 1;
  //}

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
    egeDebug(KResourceTextureDebugName) << "Aquired" << group()->name() << "for" << m_resourceRequestId;
    ege_disconnect(app()->graphics()->hardwareResourceProvider(), requestComplete, this, ResourceTexture::onRequestComplete);
    m_resourceRequestId = 0;

    // set state
    m_state = STATE_LOADED;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceTexture::loadTextureData(const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  // get XML element text
  String text = tag->text();

  // remove all spaces
  text.replaceAll(" ", "");

  // get length of data
  u32 length = static_cast<u32>(text.length());

  // allocate buffer
  PDataBuffer buffer = ege_new DataBuffer();
  if (NULL == buffer)
  {
    // error!
    result = EGE_ERROR_NO_MEMORY;
  }

  // convert according to data type
  if ((EGE_SUCCESS == result) && ("hex" == m_dataType))
  {
    // NOTE: Hex data type consists of a series of 2-character words where each encodes a single byte.    

    // check if length is odd
    if (length & 0x1)
    {
      // error!
      result = EGE_ERROR;
    }
    else
    {
      // allocate sufficient memory in a buffer
      result = buffer->setSize(static_cast<s64>(length) / 2);
    }

    if (EGE_SUCCESS == result)
    {
      u8* data = reinterpret_cast<u8*>(buffer->data());

      // decode data
      bool error = false;
      for (u32 i = 0; (i < length) && ! error; i += 2)
      {
        // decode
        char encodedByte[3] = { text[i], text[i + 1], 0 };
        u8 byte = StringUtils::FromHex(encodedByte, &error);

        // store
        *data++ = byte;
      }

      // check for error
      if (error)
      {
        // error!
        result = EGE_ERROR;
      }
      else
      {
        // set data buffer
        m_data = buffer;
      }
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END