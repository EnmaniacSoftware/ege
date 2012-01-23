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
static const Rectf l_defaultTitleAreaRect = Rectf(0, 0.5f, 1.0f, 0.2f);
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
    m_titleFont = fontResource->font();
    m_textFont  = fontResource->font();

    // title label
    PLabel label = app->graphics()->widgetFactory()->createWidget("label", "title");
    if (label)
    {
      //label->setTextAlignment(ALIGN_CENTER);
      label->setFont(m_titleFont);
      addChild(label);
    }

    // text label
    label = app->graphics()->widgetFactory()->createWidget("label", "text");
    if (label)
    {
      label->setTextAlignment(ALIGN_HCENTER);
      label->setFont(m_textFont);
      addChild(label);
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
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets content text. */
void Dialog::setText(const Text& text)
{
  PLabel label = child("text");
  if (NULL != label)
  {
    EGE_ASSERT(EGE_OBJECT_UID_UI_LABEL == label->uid());

    label->setText(text);
  }
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

  // TITLE AREA
  if (params.contains("title-rect"))
  {
    PLabel label = child("title");

    Rectf rect = params.at("title-rect").toRectf(&error);
    
    label->setPosition(Vector4f(rect.x, rect.y, 0));
    label->setSize(Vector2f(rect.width, rect.height));
  }

  if (params.contains("title-font"))
  {
    PResourceFont fontResource = app()->resourceManager()->resource(RESOURCE_NAME_FONT, params.at("title-font"));
    if (fontResource)
    {
      // set new font
      setTitleFont(fontResource->font());
    }
    else
    {
      // error!
      error = true;
    }
  }

  // TEXT AREA
  if (params.contains("text-rect"))
  {
    PLabel label = child("text");

    Rectf rect = params.at("text-rect").toRectf(&error);

    label->setPosition(Vector4f(rect.x, rect.y, 0));
    label->setSize(Vector2f(rect.width, rect.height));
  }

  if (params.contains("text-font"))
  {
    PResourceFont fontResource = app()->resourceManager()->resource(RESOURCE_NAME_FONT, params.at("text-font"));
    if (fontResource)
    {
      // set new font
      setTextFont(fontResource->font());
    }
    else
    {
      // error!
      error = true;
    }
  }

  return !error;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets title font. */
void Dialog::setTitleFont(PFont font)
{
  if (font != m_titleFont)
  {
    // store fonts
    m_titleFont = font;

    // title label
    PLabel label = child("title");
    if (label)
    {
      EGE_ASSERT(EGE_OBJECT_UID_UI_LABEL == label->uid());

      label->setFont(m_titleFont);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets text font. */
void Dialog::setTextFont(PFont font)
{
  if (font != m_textFont)
  {
    // store fonts
    m_textFont = font;

    // text label
    PLabel label = child("text");
    if (label)
    {
      EGE_ASSERT(EGE_OBJECT_UID_UI_LABEL == label->uid());

      label->setFont(m_textFont);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
