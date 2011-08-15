#include "Core/Resource/ResourceFont.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/ResourceMaterial.h"
#include "Core/Graphics/Font.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(ResourceFont)
EGE_DEFINE_DELETE_OPERATORS(ResourceFont)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceFont::ResourceFont(Application* app, ResourceManager* manager) : IResource(app, manager, "font")
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceFont::~ResourceFont()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of resource. This method is a registration method for manager. */
PResource ResourceFont::Create(Application* app, ResourceManager* manager)
{
  return ege_new ResourceFont(app, manager);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of resource embedding given font object. This is helper method for manual font adding. */
PResource ResourceFont::Create(Application* app, ResourceManager* manager, const String& name, PFont font)
{
  // create empty resource
  PResourceFont resource = Create(app, manager);
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
/*! Returns TRUE if resource is loaded. */
bool ResourceFont::isLoaded() const
{
  return NULL != m_font;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Initializes resource from XML. 
* 
*  \param  path  full path to resource definition file.
*  \param  tag   xml element with resource definition. 
*/
EGEResult ResourceFont::create(const String& path, const PXmlElement& tag)
{
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
    PResourceMaterial material = manager()->resource("material", materialName());
    if (material)
    {
      // load material
      if (EGE_SUCCESS != (result = material->load()))
      {
        // error!
        return result;
      }

      font->setMaterial(material->material());
      m_font = font;
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
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

