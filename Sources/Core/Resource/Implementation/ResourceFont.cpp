#include "Core/Resource/Interface/ResourceFont.h"
#include "Core/Resource/Interface/ResourceMaterial.h"
#include "Core/Graphics/Font.h"
#include "EGEEngine.h"
#include "EGEResources.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KResourceFontDebugName = "EGEResourceFont";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceFont)
EGE_DEFINE_DELETE_OPERATORS(ResourceFont)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceFont::ResourceFont(Engine& engine, ResourceGroup* group) 
: IResource(engine, group, RESOURCE_NAME_FONT)
, m_height(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceFont::~ResourceFont()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PResource ResourceFont::Create(Engine& engine, ResourceGroup* group)
{
  return ege_new ResourceFont(engine, group);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PResource ResourceFont::Create(Engine& engine, ResourceGroup* group, const String& name, PFont font)
{
  // create empty resource
  PResourceFont resource = Create(engine, group);
  if (resource)
  {
    resource->m_name    = name;
    resource->m_font    = font;
    resource->m_manual  = true;
    resource->m_height  = font->height();
  }

  return resource;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& ResourceFont::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceFont::create(const String& path, const PXmlElement& tag)
{
  EGE_UNUSED(path);

  EGEResult result = EGE_SUCCESS;

  bool error = false;

  // get data
  m_name         = tag->attribute("name");
  m_materialName = tag->attribute("material");
  m_height       = tag->attribute("height").toInt(&error);

  // check if obligatory data is wrong
  if (m_name.empty() || m_materialName.empty() || error)
  {
    // error!
    egeDebug(KResourceFontDebugName) << "Failed for name:" << m_name;
    return EGE_ERROR_BAD_PARAM;
  }

  PXmlElement child = tag->firstChild();
  while (child->isValid())
  {
    // check if GLYPH element
    if ("glyph" == child->name())
    {
      GlyphData glyph;

      error = false;

      u16 value                   = (u16) child->attribute("value").toInt(&error);
      glyph.m_textureRect.x       = child->attribute("image-x").toFloat(&error);
      glyph.m_textureRect.y       = child->attribute("image-y").toFloat(&error);
      glyph.m_textureRect.width   = child->attribute("image-width").toFloat(&error);
      glyph.m_textureRect.height  = child->attribute("image-height").toFloat(&error);
      glyph.m_width               = child->attribute("width").toInt(&error);

      // check for errors
      if (error)
      {
        // error!
        return EGE_ERROR;
      }

      // add to pool
      m_glyphs.insert(value, glyph);
    }

    // go to next element
    child = child->nextChild();
  }

  // check if success
  if (EGE_SUCCESS == result)
  {
    // set state
    m_state = STATE_UNLOADED;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceFont::load()
{
  EGEResult result = EGE_SUCCESS;

  if (STATE_LOADED != m_state)
  {
    // get material
    PResourceMaterial materialResource = engine().resourceManager()->resource(RESOURCE_NAME_MATERIAL, materialName());
    if (materialResource)
    {
      // load material
      if (EGE_SUCCESS != (result = materialResource->load()))
      {
        // error!
        return result;
      }

      PMaterial material = materialResource->createInstance();
      if (NULL == material)
      {
        // error!
        return EGE_ERROR;
      }

      PFont font = ege_new Font(height(), m_glyphs);
      if (NULL == font)
      {
        // error!
        return EGE_ERROR_NO_MEMORY;
      }

      // set font material
      font->setMaterial(material);

      // store font
      m_font = font;
    
      // set flag
      m_state = STATE_LOADED;
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
void ResourceFont::unload()
{
  // clean up
  m_font = NULL;
  
  // reset flag
  m_state = STATE_UNLOADED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END