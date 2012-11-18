#include "Core/Resource/ResourceWidget.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/UI/WidgetFrame.h"
#include "Core/UI/WidgetFactory.h"
#include <EGEApplication.h>
#include <EGEResources.h>
#include <EGEGraphics.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define NODE_CHILD "child"
#define NODE_FRAME "frame"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceWidget)
EGE_DEFINE_DELETE_OPERATORS(ResourceWidget)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceWidget::ResourceWidget(Application* app, ResourceGroup* group) : IResource(app, group, RESOURCE_NAME_WIDGET)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceWidget::~ResourceWidget()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of resource. This method is a registration method for manager. */
PResource ResourceWidget::Create(Application* app, ResourceGroup* group)
{
  return ege_new ResourceWidget(app, group);
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

  // parse all attributes
  PXmlAttribute attribute = tag->firstAttribute();
  while (attribute)
  {
    // store data
    m_parameters[attribute->name()] = attribute->value();

    // go to next attribute
    attribute = attribute->next();
  }

  bool error = false;
  m_name = m_parameters.value("name", "no name");

  // check if obligatory data is wrong
  if (error || !m_parameters.contains("name") || !m_parameters.contains("type") || !m_parameters.contains("size"))
  {
    // error!
    egeWarning() << "Failed for name:" << m_name;
    return EGE_ERROR_BAD_PARAM;
  }

  // go thru all sub node
  PXmlElement child = tag->firstChild();
  while (child->isValid())
  {
    // check child
    if (NODE_CHILD == child->name())
    {
      // process child
      result = processChild(child);
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

  // set flag
  m_loaded = true;

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Unloads resource. */
void ResourceWidget::unload()
{
  // reset flag
  m_loaded = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of widget object defined by resource. */
PWidget ResourceWidget::createInstance()
{
  // create instance of widget of a correct type and with given name
  PWidget object = app()->graphics()->widgetFactory()->createWidget(m_parameters["type"], m_name);
  if (object)
  {
    // initialize with dictionary
    if (!object->initialize(m_parameters))
    {
      // error!
      egeWarning() << "Could not initialize!";
      return NULL;
    }

    // set frame rects
    if (object->widgetFrame())
    {
      object->widgetFrame()->setRects(m_frameData.topLeftRect, m_frameData.topMiddleRect, m_frameData.topRightRect, 
                                      m_frameData.middleLeftRect, m_frameData.fillRect, m_frameData.middleRightRect, 
                                      m_frameData.bottomLeftRect, m_frameData.bottomMiddleRect, m_frameData.bottomRightRect);
    }

    // add children
    for (ChildDataList::const_iterator it = m_children.begin(); it != m_children.end(); ++it)
    {
      const ChildData& childData = *it;

      PResourceWidget widgetRes = app()->resourceManager()->resource(RESOURCE_NAME_WIDGET, childData.widgetName);
      EGE_ASSERT(widgetRes);
      if (widgetRes)
      {
        PWidget childWidget = widgetRes->createInstance();
        EGE_ASSERT(childWidget);
        if (childWidget)
        {
          // set name
          childWidget->setName(childData.name);
          childWidget->setPosition(Vector4f(childData.offset.x, childData.offset.y, 0));

          // add to child pool
          if (EGE_SUCCESS != object->addChild(childWidget))
          {
            // error!
            egeWarning() << "Could not add child widget with name" << childData.name;
            return NULL;
          }
        }
        else
        {
          // error!
          egeWarning() << "Could not create" << childData.widgetName << "child widget.";
          return NULL;
        }
      }
      else
      {
        // error!
        egeWarning() << "Could not find" << childData.widgetName << "child widget resource";
        return NULL;
      }
    }
  }

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Processes child data. */
EGEResult ResourceWidget::processChild(const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  bool error = false;

  ChildData childData;

  // retrieve data
  childData.name        = tag->attribute("name");
  childData.offset      = tag->attribute("offset", "0 0").toVector2f(&error);
  childData.widgetName  = tag->attribute("widget-name");
 
  // check for error
  if (childData.name.empty() || childData.widgetName.empty() || error)
  {
    // error!
    egeWarning() << "Failed for name:" << m_name;
    return EGE_ERROR_BAD_PARAM;
  }

  // check if child with such name exists
  for (ChildDataList::const_iterator it = m_children.begin(); it != m_children.end(); ++it)
  {
    if ((*it).name == childData.name)
    {
      // error!
      return EGE_ERROR_ALREADY_EXISTS;
    }
  }

  // add to pool
  m_children.push_back(childData);

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
    egeWarning() << "Failed for name:" << m_name;
    return EGE_ERROR_BAD_PARAM;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END