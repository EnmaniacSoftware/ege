#include "Core/UI/PushButton.h"
#include "Core/UI/WidgetFactory.h"
#include <EGEApplication.h>
#include <EGEResources.h>
#include <EGEGraphics.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(PushButton)
EGE_DEFINE_DELETE_OPERATORS(PushButton)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const Rectf l_defaultTextAreaRect = Rectf(0.1f, 0.1f, 0.8f, 0.8f);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PushButton::PushButton(Application* app, const String& name, egeObjectDeleteFunc deleteFunc) : Widget(app, name, EGE_OBJECT_UID_UI_PUSH_BUTTON, deleteFunc),
                                                                                               m_clicked(false)
{
  PResourceFont fontResource = app->resourceManager()->resource(RESOURCE_NAME_FONT, "debug-font");
  if (fontResource)
  {
    // store used font
    m_font = fontResource->font();

    // title label
    PLabel label = app->graphics()->widgetFactory()->createWidget("label", "text");
    if (label)
    {
      label->setFont(m_font);
      addChild(label);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PushButton::~PushButton()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of widget. This method is a registration method for factory. */
PWidget PushButton::Create(Application* app, const String& name)
{
  return ege_new PushButton(app, name);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Widget override. Returns TRUE if object is valid. */
bool PushButton::isValid() const
{
  return Widget::isValid() && (NULL != child("text"));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Widget override. Updates overlay. */
void PushButton::update(const Time& time)
{
  // call base class
  Widget::update(time);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Widget override. Renders dialog. */
void PushButton::addForRendering(Renderer* renderer, const Matrix4f& transform)
{
  if (isVisible())
  {
    // call base class
    Widget::addForRendering(renderer, transform);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Pointer event processor. */
void PushButton::pointerEvent(PPointerData data)
{
  const Matrix4f& globalMatrix = globalTransformationMatrix();

  Vector4f pos = globalMatrix.translation();

  Rectf rect(pos.x, pos.y, size().x, size().y);

  // check if inside
  if (rect.contains(static_cast<float32>(data->x()), static_cast<float32>(data->y())))
  {
    switch (data->action())
    {
      case ACTION_BUTTON_DOWN:

        // store flag
        m_clicked = true;
        break;

      case ACTION_BUTTON_UP:

        // check if was clicked
        if (m_clicked)
        {
          // emit
          emit clicked(this);
        }
        break;
    }
  }

  // reset click flag if released
  if (ACTION_BUTTON_UP == data->action())
  {
    m_clicked = false;
  }

  // call base class
  Widget::pointerEvent(data);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Generates render data. */
void PushButton::generateRenderData()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Widget override. Returns TRUE if widget is frameless. */
bool PushButton::isFrameless() const
{
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Widget override. Initializes widget from dictionary. */
bool PushButton::initialize(const Dictionary& params)
{
  // initialize base
  bool error = !Widget::initialize(params);

  PLabel label = child("text");

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
    }
  }
  else
  {
    // error!
    error = true;
  }

  // check if font name is defined
  if (params.contains("font"))
  {
    PResourceFont fontResource = app()->resourceManager()->resource(RESOURCE_NAME_FONT, params.at("font"));
    if (fontResource)
    {
      // set new font
      setFont(fontResource->font());
    }
    else
    {
      // error!
      error = true;
    }
  }

  if (params.contains("text-alignment"))
  {
    Alignment alignment = params.at("text-alignment").toAlignment(&error);
    label->setTextAlignment(alignment);
  }

  return !error;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets content text. */
void PushButton::setText(const Text& text)
{
  PLabel label = child("text");
  if (NULL != label)
  {
    EGE_ASSERT(EGE_OBJECT_UID_UI_LABEL == label->uid());

    label->setText(text);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Detrmines widget's content size (in pixels). */
Vector2f PushButton::contentSize()
{
  return child("text")->contentSize();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Set font. */
void PushButton::setFont(PFont font)
{
  if (font != m_font)
  {
    // title label
    PLabel label = child("text");
    if (label)
    {
      EGE_ASSERT(EGE_OBJECT_UID_UI_LABEL == label->uid());

      label->setFont(m_font);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END