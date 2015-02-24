#include "Core/Resource/Interface/ResourceWidget.h"
#include "Core/UI/WidgetFrame.h"
#include "Core/UI/WidgetFactory.h"
#include "EGEEngine.h"
#include "EGEResourceManager.h"
#include "EGEResources.h"
#include "EGEGraphics.h"
#include "EGEStringUtils.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KResourceWidgetDebugName = "EGEResourceWidget";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define NODE_CHILD "child"
#define NODE_FRAME "frame"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceWidget)
EGE_DEFINE_DELETE_OPERATORS(ResourceWidget)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceWidget::ResourceWidget(Engine& engine, ResourceGroup* group) 
: IResource(engine, group, RESOURCE_NAME_WIDGET)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceWidget::~ResourceWidget()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& ResourceWidget::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceWidget::create(const String& path, const PObject& data)
{
  EGE_UNUSED(path);

  EGEResult result = EGE_SUCCESS;

  PXmlElement tag = ege_pcast<PXmlElement>(data);

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
    egeWarning(KResourceWidgetDebugName) << "Failed for name:" << m_name;
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

  // check if success
  if (EGE_SUCCESS == result)
  {
    // set state
    m_state = STATE_UNLOADED;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceWidget::load()
{
  // set flag
  m_state = STATE_LOADED;

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceWidget::unload()
{
  // reset flag
  m_state = STATE_UNLOADED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PWidget ResourceWidget::createInstance()
{
  // create instance of widget of a correct type and with given name
  PWidget object = engine().graphics()->widgetFactory()->createWidget(m_parameters["type"], m_name);
  if (NULL != object)
  {
    // initialize with dictionary
    if ( ! object->initialize(m_parameters))
    {
      // error!
      egeWarning(KResourceWidgetDebugName) << "Could not initialize!";
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
    for (ChildDataList::ConstIterator it = m_children.begin(); it != m_children.end(); ++it)
    {
      const ChildData& childData = *it;

      PResourceWidget widgetRes = engine().resourceManager()->resource(RESOURCE_NAME_WIDGET, childData.widgetName);
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
            egeWarning(KResourceWidgetDebugName) << "Could not add child widget with name" << childData.name;
            return NULL;
          }
        }
        else
        {
          // error!
          egeWarning(KResourceWidgetDebugName) << "Could not create" << childData.widgetName << "child widget.";
          return NULL;
        }
      }
      else
      {
        // error!
        egeWarning(KResourceWidgetDebugName) << "Could not find" << childData.widgetName << "child widget resource";
        return NULL;
      }
    }
  }

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceWidget::processChild(const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  bool error = false;

  ChildData childData;

  // retrieve data
  childData.name        = tag->attribute("name");
  childData.offset      = StringUtils::ToVector2f(tag->attribute("offset", "0 0"), &error);
  childData.widgetName  = tag->attribute("widget-name");
 
  // check for error
  if (childData.name.isEmpty() || childData.widgetName.isEmpty() || error)
  {
    // error!
    egeWarning(KResourceWidgetDebugName) << "Failed for name:" << m_name;
    return EGE_ERROR_BAD_PARAM;
  }

  // check if child with such name exists
  for (ChildDataList::ConstIterator it = m_children.begin(); it != m_children.end(); ++it)
  {
    if ((*it).name == childData.name)
    {
      // error!
      return EGE_ERROR_ALREADY_EXISTS;
    }
  }

  // add to pool
  m_children.append(childData);

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceWidget::processFrame(const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  bool error = false;

  // retrieve data
  m_frameData.topLeftRect       = StringUtils::ToRecti(tag->attribute("top-left-rect"), &error);
  m_frameData.topMiddleRect     = StringUtils::ToRecti(tag->attribute("top-middle-rect"), &error);
  m_frameData.topRightRect      = StringUtils::ToRecti(tag->attribute("top-right-rect"), &error);
  m_frameData.middleLeftRect    = StringUtils::ToRecti(tag->attribute("middle-left-rect"), &error);
  m_frameData.fillRect          = StringUtils::ToRecti(tag->attribute("fill-rect"), &error);
  m_frameData.middleRightRect   = StringUtils::ToRecti(tag->attribute("middle-right-rect"), &error);
  m_frameData.bottomLeftRect    = StringUtils::ToRecti(tag->attribute("bottom-left-rect"), &error);
  m_frameData.bottomMiddleRect  = StringUtils::ToRecti(tag->attribute("bottom-middle-rect"), &error);
  m_frameData.bottomRightRect   = StringUtils::ToRecti(tag->attribute("bottom-right-rect"), &error);

  // check for error
  if (error)
  {
    // error!
    egeWarning(KResourceWidgetDebugName) << "Failed for name:" << m_name;
    return EGE_ERROR_BAD_PARAM;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END