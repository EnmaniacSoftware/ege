#include "Core/Resource/Interface/ResourceTexture.h"
#include "Core/Graphics/Graphics.h"
#include "Core/Graphics/HardwareResourceProvider.h"
#include "Core/Graphics/Render/RenderSystem.h"
#include "EGEEngine.h"
#include "EGEGraphics.h"
#include "EGEXml.h"
#include "EGEResources.h"
#include "EGEStringUtils.h"
#include "EGEDirectory.h"
#include "EGEAtomic.h"
#include "EGEDebug.h"

EGE_NAMESPACE

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
ResourceTexture::ResourceTexture(Engine& engine, ResourceGroup* group) 
: IResource(engine, group, RESOURCE_NAME_TEXTURE)
, m_minFilter(TF_NEAREST)
, m_magFilter(TF_NEAREST)
, m_addressingModeS(AM_REPEAT)
, m_addressingModeT(AM_REPEAT)
, m_mipmap(false)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceTexture::~ResourceTexture()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PResource ResourceTexture::Create(Engine& engine, ResourceGroup* group, const String& name, PObject texture)
{
  // create empty resource
  PResourceTexture resource = ege_new ResourceTexture(engine, group);
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
EGEResult ResourceTexture::create(const String& path, const PObject& data)
{
  EGEResult result = EGE_SUCCESS;

  PXmlElement tag = ege_pcast<PXmlElement>(data);

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
  m_mipmap          = tag->attribute("mipmap", "false").toBool(&error);

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

  EGE_ASSERT_X(m_mipmap || ( ! m_mipmap && (TF_TRILINEAR != m_minFilter)), "Trilinear filtering can be used with mipmaps only!");
  EGE_ASSERT_X(TF_TRILINEAR != m_magFilter, "Trilinear filtering can be used with minification filtering only!");

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
      // create texture
      result = create2D();
      
      // check if success and that is hasnt been loaded in the meantime
      if (EGE_SUCCESS == result)
      {
        // set to loading
        egeAtomicCompareAndSet(reinterpret_cast<volatile s32&>(m_state), STATE_UNLOADED, STATE_LOADING);
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
  engine().graphics()->renderSystem()->setTextureMinFilter(minFilter());
  engine().graphics()->renderSystem()->setTextureMagFilter(magFilter());
  engine().graphics()->renderSystem()->setTextureAddressingModeS(adressingModeS());
  engine().graphics()->renderSystem()->setTextureAddressingModeT(adressingModeT());
  engine().graphics()->renderSystem()->setTextureMipMapping(mipmap());

  // request texture
  bool result = engine().graphics()->hardwareResourceProvider()->requestCreateTexture2D(name(), image, ege_make_slot(this, ResourceTexture::onRequestComplete));
  
  return result ? EGE_SUCCESS :  EGE_ERROR;
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
      engine().graphics()->hardwareResourceProvider()->requestDestroyTexture2D(m_texture);

      // clean up
      m_texture = NULL; 
    }

    // reset flag
    m_state = STATE_UNLOADED;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceTexture::onRequestComplete(PObject object)
{
  // store handle
  m_texture = object;

  // set state
  egeAtomicCompareAndSet(reinterpret_cast<volatile s32&>(m_state), m_state, STATE_LOADED);
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
const String& ResourceTexture::type() const 
{ 
  return m_type; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureFilter ResourceTexture::minFilter() const 
{ 
  return m_minFilter; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureFilter ResourceTexture::magFilter() const 
{ 
  return m_magFilter; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureAddressingMode ResourceTexture::adressingModeS() const 
{ 
  return m_addressingModeS; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureAddressingMode ResourceTexture::adressingModeT() const 
{ 
  return m_addressingModeT; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResourceTexture::mipmap() const
{
  return m_mipmap; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PObject ResourceTexture::texture() const 
{ 
  return m_texture; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
