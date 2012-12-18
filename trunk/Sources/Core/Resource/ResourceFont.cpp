#include "Core/Resource/ResourceFont.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/ResourceMaterial.h"
#include "Core/Graphics/Font.h"
#include <EGEResources.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceFont)
EGE_DEFINE_DELETE_OPERATORS(ResourceFont)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceFont::ResourceFont(Application* app, ResourceGroup* group) : IResource(app, group, RESOURCE_NAME_FONT)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceFont::~ResourceFont()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of resource. This method is a registration method for manager. */
PResource ResourceFont::Create(Application* app, ResourceGroup* group)
{
  return ege_new ResourceFont(app, group);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of resource embedding given font object. This is helper method for manual font adding. */
PResource ResourceFont::Create(Application* app, ResourceGroup* group, const String& name, PFont font)
{
  // create empty resource
  PResourceFont resource = Create(app, group);
  if (resource)
  {
    resource->m_name    = name;
    resource->m_font    = font;
    resource->m_manual  = true;
  }

  return resource;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Returns name of resource. */
const String& ResourceFont::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Initializes resource from XML. 
* 
*  \param  path  full path to resource definition file.
*  \param  tag   xml element with resource definition. 
*/
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
    egeDebug() << "Failed for name:" << m_name;
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

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Loads resource. */
EGEResult ResourceFont::load()
{
  EGEResult result = EGE_SUCCESS;

  if (!isLoaded())
  {
    PFont font = ege_new Font(app(), height(), m_glyphs);
    if (NULL == font)
    {
      // error!
      return EGE_ERROR_NO_MEMORY;
    }

    // get material
    PResourceMaterial materialResource = group()->manager()->resource(RESOURCE_NAME_MATERIAL, materialName());
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

      // set font material
      font->setMaterial(material);

      // store font
      m_font = font;
    
      // set flag
      m_loaded = true;
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
void ResourceFont::unload()
{
  m_font = NULL;
  
  // reset flag
  m_loaded = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END