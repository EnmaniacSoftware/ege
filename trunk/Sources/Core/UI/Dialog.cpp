#include "Core/UI/Dialog.h"
#include "Core/UI/WidgetFactory.h"
#include <EGEApplication.h>
#include <EGEResources.h>
#include <EGEGraphics.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Dialog)
EGE_DEFINE_DELETE_OPERATORS(Dialog)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const Rectf l_defaultTitleAreaRect = Rectf(0, 0, 1.0f, 0.2f);
static const Rectf l_defaultTextAreaRect  = Rectf(0, 0.2f, 1.0f, 0.8f);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Dialog::Dialog(Application* app, const String& name, egeObjectDeleteFunc deleteFunc) : Widget(app, name, EGE_OBJECT_UID_UI_DIALOG, deleteFunc)
{
  m_renderData  = ege_new RenderComponent(app, "dialog-" + name, EGEGraphics::RP_MAIN, EGEGraphics::RPT_TRIANGLES, EGEVertexBuffer::UT_STATIC_WRITE);
  if (m_renderData)
  {
    m_renderData->indexBuffer()->setIndexSize(EGEIndexBuffer::IS_8BIT);
    if (!m_renderData->vertexBuffer()->setSemantics(EGEVertexBuffer::ST_V2_T2))
    {
      // error!
      m_renderData = NULL;
    }
  }

  PResourceFont fontResource = app->resourceManager()->resource(RESOURCE_NAME_FONT, "debug-font");
  if (fontResource)
  {
    // title label
    PLabel label = app->graphics()->widgetFactory()->createWidget("label", "title");
    if (label)
    {
      label->setFont(fontResource->font());
      addChild(label, l_defaultTitleAreaRect);
    }

    // text scrollable area
    PScrollableArea scrollableArea = app->graphics()->widgetFactory()->createWidget("scrollable-area", "text");
    if (scrollableArea)
    {
      PTextOverlay textOverlay = ege_new TextOverlay(app, "text");
      if (textOverlay)
      {
        textOverlay->setFont(fontResource->font());
        scrollableArea->addObject(textOverlay);
        addChild(scrollableArea, l_defaultTextAreaRect);
      }

      // set default behavior
      scrollableArea->setDirection(ScrollableArea::DIRECTION_NONE);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Dialog::~Dialog()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of widget. This method is a registration method for factory. */
PWidget Dialog::Create(Application* app, const String& name)
{
  return ege_new Dialog(app, name);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Widget override. Returns TRUE if object is valid. */
bool Dialog::isValid() const
{
  return Widget::isValid() && (NULL != child("title")) && (NULL != child("text"));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Widget override. Updates overlay. */
void Dialog::update(const Time& time)
{
  // call base class
  Widget::update(time);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Widget override. Renders dialog. */
void Dialog::addForRendering(Renderer* renderer, const Matrix4f& transform)
{
  // call base class
  Widget::addForRendering(renderer, transform);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Pointer event processor. */
void Dialog::pointerEvent(PPointerData data)
{
  // call base class
  Widget::pointerEvent(data);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Generates render data. */
void Dialog::generateRenderData()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets title text. */
void Dialog::setTitle(const Text& title)
{
  PLabel label = child("title");
  if (NULL != label)
  {
    EGE_ASSERT(EGE_OBJECT_UID_UI_LABEL == label->uid());

    label->setText(title);
  }

  // invalidate size
  m_sizeValid = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets content text. */
void Dialog::setText(const Text& text)
{
  PScrollableArea scrollableArea = child("text");
  if (NULL != scrollableArea)
  {
    EGE_ASSERT(EGE_OBJECT_UID_UI_SCROLLABLE_AREA == scrollableArea->uid());

    PTextOverlay textOverlay = scrollableArea->object("text");
    if (NULL != textOverlay)
    {
      EGE_ASSERT(EGE_OBJECT_UID_OVERLAY_TEXT == textOverlay->uid());
      textOverlay->setText(text);
    }
  }

  // invalidate size
  m_sizeValid = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Widget override. Returns TRUE if widget is frameless. */
bool Dialog::isFrameless() const
{
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Widget override. Initializes widget from dictionary. */
bool Dialog::initialize(const Dictionary& params)
{
  // initialize base
  bool error = !Widget::initialize(params);
  
  // check if material name is defined
  if (params.contains("material"))
  {
    PResourceMaterial materialResource = app()->resourceManager()->materialResource(params.at("material"));
    EGE_ASSERT(materialResource);
    if (materialResource)
    {
      PMaterial material = materialResource->createInstance();
      if (NULL == material)
      {
        // error!
        error = true;
      }
      
      // set frame material
      widgetFrame()->setMaterial(material);

      // set render data material
      m_renderData->setMaterial(material);
    }
  }
  else
  {
    // error!
    error = true;
  }

  // go thru all children
  for (ChildrenDataMap::iterator it = m_children.begin(); it != m_children.end(); ++it)
  {
  }

/*  // TITLE AREA
  if (params.contains("title-rect"))
  {
    titleArea.rect = params.at("title-area-rect").toRectf(&error);
  }

  if (params.contains("title-area-vertical-scroll"))
  {
    if (params.at("title-area-vertical-scroll").toBool(&error))
    {
      titleArea.area->setDirection(titleArea.area->direction() | ScrollableArea::DIRECTION_VERTICAL);
    }
  }

  if (params.contains("title-area-horizontal-scroll"))
  {
    if (params.at("title-area-horizontal-scroll").toBool(&error))
    {
      titleArea.area->setDirection(titleArea.area->direction() | ScrollableArea::DIRECTION_HORIZONTAL);
    }
  }

  // TEXT AREA
  if (params.contains("text-area-rect"))
  {
    titleArea.rect = params.at("text-area-rect").toRectf(&error);
  }

  if (params.contains("text-area-vertical-scroll"))
  {
    if (params.at("text-area-vertical-scroll").toBool(&error))
    {
      titleArea.area->setDirection(titleArea.area->direction() | ScrollableArea::DIRECTION_VERTICAL);
    }
  }

  if (params.contains("text-area-horizontal-scroll"))
  {
    if (params.at("text-area-horizontal-scroll").toBool(&error))
    {
      titleArea.area->setDirection(titleArea.area->direction() | ScrollableArea::DIRECTION_HORIZONTAL);
    }
  }*/
 
  return !error;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets transparency level. */
void Dialog::setAlpha(float32 alpha)
{
  m_widgetFrame->material()->setDiffuseColor(Color(1.0f, 1.0f, 1.0f, alpha));

  // go thru all children
  for (ChildrenDataMap::iterator it = m_children.begin(); it != m_children.end(); ++it)
  {
    ChildData& data = it->second;

    //data.widget->

    // TAGE - implement
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
