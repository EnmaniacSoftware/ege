#include "Core/Resource/ResourceDialog.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Graphics/Material.h"
#include <EGEResources.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define NODE_CONTENT_AREA "content-area"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceDialog)
EGE_DEFINE_DELETE_OPERATORS(ResourceDialog)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceDialog::ResourceDialog(Application* app, ResourceManager* manager) : IResource(app, manager, RESOURCE_NAME_DIALOG)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceDialog::~ResourceDialog()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of resource. This method is a registration method for manager. */
PResource ResourceDialog::Create(Application* app, ResourceManager* manager)
{
  return ege_new ResourceDialog(app, manager);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Returns name of resource. */
const String& ResourceDialog::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Initializes resource from XML. 
* 
*  \param  path  full path to resource definition file.
*  \param  tag   xml element with resource definition. 
*/
EGEResult ResourceDialog::create(const String& path, const PXmlElement& tag)
{
  EGE_UNUSED(path);

  EGEResult result = EGE_SUCCESS;

  bool error = false;

  // retrieve data
  m_name              = tag->attribute("name");
  m_materialName      = tag->attribute("material");
  m_topLeftRect       = tag->attribute("top-left-rect").toRecti(&error);
  m_topMiddleRect     = tag->attribute("top-middle-rect").toRecti(&error);
  m_topRightRect      = tag->attribute("top-right-rect").toRecti(&error);
  m_middleLeftRect    = tag->attribute("middle-left-rect").toRecti(&error);
  m_fillRect          = tag->attribute("fill-rect").toRecti(&error);
  m_middleRightRect   = tag->attribute("middle-right-rect").toRecti(&error);
  m_bottomLeftRect    = tag->attribute("bottom-left-rect").toRecti(&error);
  m_bottomMiddleRect  = tag->attribute("bottom-middle-rect").toRecti(&error);
  m_bottomRightRect   = tag->attribute("bottom-right-rect").toRecti(&error);
  m_maxSize           = tag->attribute("max-size", "0 0").toVector2i(&error);

  // check if obligatory data is wrong
  if (error || m_name.empty() || m_materialName.empty())
  {
    // error!
    EGE_PRINT("ResourceDialog::create - failed for name: %s", m_name.toAscii());
    return EGE_ERROR_BAD_PARAM;
  }

  // go thru all sub node
  PXmlElement child = tag->firstChild();
  while (child->isValid())
  {
    // check child
    if (NODE_CONTENT_AREA == child->name())
    {
      // add content area
      result = addContentArea(child);
    }

    // check if failed
    if (EGE_SUCCESS != result)
    {
      // error, done!
      break;
    }

    // go to next child
    child = child->nextChild();
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Loads resource. */
EGEResult ResourceDialog::load()
{
  EGEResult result = EGE_SUCCESS;

  if (!isLoaded())
  {
    // load material
    m_material = manager()->resource(RESOURCE_NAME_MATERIAL, m_materialName);
    if (m_material)
    {
      // load sheet
      if (EGE_SUCCESS != (result = m_material->load()))
      {
        // error!
        m_material = NULL;
        return result;
      }
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
void ResourceDialog::unload()
{
  m_material = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of material object defined by resource. */
PDialog ResourceDialog::createInstance() const
{
	PDialog object = ege_new Dialog(app(), m_name);
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
/*! Set given instance of material object to what is defined by resource. */
EGEResult ResourceDialog::setInstance(const PDialog& instance) const
{
  // sanity check
  if (NULL == instance || !isLoaded())
  {
    return EGE_ERROR;
  }

  // set rects
  instance->setRects(m_topLeftRect, m_topMiddleRect, m_topRightRect, m_middleLeftRect, m_fillRect, m_middleRightRect, m_bottomLeftRect, m_bottomMiddleRect, 
                     m_bottomRightRect);
  
  // set max size
  instance->setMaxSize(m_maxSize);

  // set material
  PMaterial material = m_material->createInstance();
  if (NULL == material)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  instance->setMaterial(material);

  // add content areas
  for (ContentAreaDataMap::const_iterator it = m_contentAreas.begin(); it != m_contentAreas.end(); ++it)
  {
    EGEResult result;
    if (EGE_SUCCESS != (result = instance->addContentArea(it->first, it->second.rect)))
    {
      // error!
      return result;
    }
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds content area. */
EGEResult ResourceDialog::addContentArea(const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  bool error = false;

  ContentAreaData area;

  // retrieve data
  String name = tag->attribute("name");
  area.rect   = tag->attribute("rect").toRectf(&error);

  // check for error
  if (name.empty() || error)
  {
    // error!
    EGE_PRINT("ResourceDialog::addContentArea - failed for name: %s", m_name.toAscii());
    return EGE_ERROR_BAD_PARAM;
  }

  // check if area with such name exists
  if (m_contentAreas.contains(name))
  {
    // error!
    return EGE_ERROR_ALREADY_EXISTS;
  }

  // add to pool
  m_contentAreas.insert(name, area);

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
