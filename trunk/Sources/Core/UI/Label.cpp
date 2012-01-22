#include "Core/UI/Label.h"
#include "Core/UI/ScrollableArea.h"
#include <EGEApplication.h>
#include <EGEResources.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Label)
EGE_DEFINE_DELETE_OPERATORS(Label)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Label::Label(Application* app, const String& name, egeObjectDeleteFunc deleteFunc) : Widget(app, name, EGE_OBJECT_UID_UI_LABEL, deleteFunc)
{
  // set default font
  PResourceFont fontResource = app->resourceManager()->resource(RESOURCE_NAME_FONT, "debug-font");
  if (fontResource)
  {
    // add text overlays
    m_textOverlay  = ege_new TextOverlay(app, "text");

    if (NULL != m_textOverlay)
    {
      m_textOverlay->setFont(fontResource->font());

      m_textOverlay->physics()->setPosition(Vector4f(0, 0, 0));
    }
  }  
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Label::~Label()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of widget. This method is a registration method for factory. */
PWidget Label::Create(Application* app, const String& name)
{
  return ege_new Label(app, name);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Widget override. Returns TRUE if object is valid. */
bool Label::isValid() const
{
  return Widget::isValid() && (NULL != m_textOverlay);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Widget override. Updates overlay. */
void Label::update(const Time& time)
{
  // call base class
  Widget::update(time);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Widget override. Renders dialog. */
void Label::addForRendering(Renderer* renderer, const Matrix4f& transform)
{
  renderer->addForRendering(m_textOverlay->renderData(), transform);

  // call base class
  Widget::addForRendering(renderer, transform);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Pointer event processor. */
void Label::pointerEvent(PPointerData data)
{
  // call base class
  Widget::pointerEvent(data);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Widget override. Returns TRUE if widget is frameless. */
bool Label::isFrameless() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Widget override. Initializes widget from dictionary. */
bool Label::initialize(const Dictionary& params)
{
  // initialize base
  bool error = !Widget::initialize(params);

  // process font
  if (params.contains("font"))
  {
    PResourceFont fontResource = app()->resourceManager()->resource(RESOURCE_NAME_FONT, params.at("font"));
    if (fontResource)
    {
      setFont(fontResource->font());
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
/*! Sets content text. */
void Label::setText(const Text& text)
{
  m_textOverlay->setText(text);

  // invalidate size
  m_sizeValid = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets font. */
void Label::setFont(PFont font)
{
  m_textOverlay->setFont(font);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Detrmines widget's content size (in pixels). */
Vector2f Label::contentSize()
{
  return m_textOverlay->textSize();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
