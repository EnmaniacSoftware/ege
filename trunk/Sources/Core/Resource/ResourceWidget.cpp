#include "Core/Resource/ResourceWidget.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Graphics/Material.h"
#include "Core/UI/WidgetFrame.h"
#include "Core/UI/WidgetFactory.h"
#include <EGEApplication.h>
#include <EGEResources.h>
#include <EGEGraphics.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define NODE_CONTENT_AREA "content-area"
#define NODE_FRAME        "frame"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceWidget)
EGE_DEFINE_DELETE_OPERATORS(ResourceWidget)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceWidget::ResourceWidget(Application* app, ResourceManager* manager) : IResource(app, manager, RESOURCE_NAME_WIDGET)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceWidget::~ResourceWidget()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of resource. This method is a registration method for manager. */
PResource ResourceWidget::Create(Application* app, ResourceManager* manager)
{
  return ege_new ResourceWidget(app, manager);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Returns name of resource. */
const String& ResourceWidget::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Initializes resource from XML. 
* 
*  \param  path  full path to resource definition file.
*  \param  tag   xml element with resource definition. 
*/
EGEResult ResourceWidget::create(const String& path, const PXmlElement& tag)
{
  EGE_UNUSED(path);

  EGEResult result = EGE_SUCCESS;

  bool error = false;

  // retrieve data
  m_name              = tag->attribute("name");
  m_materialName      = tag->attribute("material");
  m_maxSize           = tag->attribute("max-size", "0 0").toVector2i(&error);
  m_type              = tag->attribute("type");

  // check if obligatory data is wrong
  if (error || m_name.empty() || m_materialName.empty() || m_type.empty())
  {
    // error!
    EGE_PRINT("ERROR: Failed for name: %s", m_name.toAscii());
    return EGE_ERROR_BAD_PARAM;
  }

  // go thru all sub node
  PXmlElement child = tag->firstChild();
  while (child->isValid())
  {
    // check child
    if (NODE_CONTENT_AREA == child->name())
    {
      // process content area
      result = processContentArea(child);
    }
    else if (NODE_FRAME == child->name())
    {
      // process frame
      result = processFrame(child);
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
EGEResult ResourceWidget::load()
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
      EGE_PRINT("ERROR: Could not find material %s", m_materialName.toAscii());
      result = EGE_ERROR_NOT_FOUND;
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Unloads resource. */
void ResourceWidget::unload()
{
  m_material = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of widget object defined by resource. */
PWidget ResourceWidget::createInstance() const
{
  // create instance of widget of a correct type and with given name
  PWidget object = app()->graphics()->widgetFactory()->createWidget(m_type, m_name);
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
EGEResult ResourceWidget::setInstance(const PWidget& instance) const
{
  // sanity check
  if (NULL == instance || !isLoaded())
  {
    return EGE_ERROR;
  }

  // set rects
  instance->widgetFrame()->setRects(m_frameData.topLeftRect, m_frameData.topMiddleRect, m_frameData.topRightRect, 
                                    m_frameData.middleLeftRect, m_frameData.fillRect, m_frameData.middleRightRect, 
                                    m_frameData.bottomLeftRect, m_frameData.bottomMiddleRect, m_frameData.bottomRightRect);
  
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
    if (EGE_SUCCESS != (result = instance->addContentArea(it->first, it->second.rect, it->second.vericalScroll, it->second.horizontalScroll)))
    {
      // error!
      return result;
    }
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Processes content area. */
EGEResult ResourceWidget::processContentArea(const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  bool error = false;

  ContentAreaData area;

  // retrieve data
  String name           = tag->attribute("name");
  area.rect             = tag->attribute("rect").toRectf(&error);
  area.vericalScroll    = tag->attribute("vertical-scroll", "false").toBool(&error);
  area.horizontalScroll = tag->attribute("horizontal-scroll", "false").toBool(&error);

  // check for error
  if (name.empty() || error)
  {
    // error!
    EGE_PRINT("ERROR: Failed for name: %s", m_name.toAscii());
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
/*! Processes widget frame data. */
EGEResult ResourceWidget::processFrame(const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  bool error = false;

  // retrieve data
  m_frameData.topLeftRect       = tag->attribute("top-left-rect").toRecti(&error);
  m_frameData.topMiddleRect     = tag->attribute("top-middle-rect").toRecti(&error);
  m_frameData.topRightRect      = tag->attribute("top-right-rect").toRecti(&error);
  m_frameData.middleLeftRect    = tag->attribute("middle-left-rect").toRecti(&error);
  m_frameData.fillRect          = tag->attribute("fill-rect").toRecti(&error);
  m_frameData.middleRightRect   = tag->attribute("middle-right-rect").toRecti(&error);
  m_frameData.bottomLeftRect    = tag->attribute("bottom-left-rect").toRecti(&error);
  m_frameData.bottomMiddleRect  = tag->attribute("bottom-middle-rect").toRecti(&error);
  m_frameData.bottomRightRect   = tag->attribute("bottom-right-rect").toRecti(&error);

  // check for error
  if (error)
  {
    // error!
    EGE_PRINT("ERROR: Failed for name: %s", m_name.toAscii());
    return EGE_ERROR_BAD_PARAM;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
