#include "Core/UI/Label.h"
#include <EGEApplication.h>
#include <EGEResources.h>

EGE_NAMESPACE_BEGIN

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
    m_textOverlay = ege_new TextOverlay(app, "text");

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
/*! Widget override. Updates widget. */
void Label::update(const Time& time)
{
  // call base class
  Widget::update(time);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Widget override. Renders widget. */
void Label::addForRendering(Renderer* renderer, const Matrix4f& transform)
{
  if (isVisible())
  {
    // apply alignment to text overlay
    Vector4f overlayPos(0, 0, 0);
    Math::Align(&overlayPos, &m_size, ALIGN_TOP_LEFT, m_textOverlay->alignment());

    Matrix4f matrix;
    Vector4f pos(m_physics.position().x + overlayPos.x, m_physics.position().y + overlayPos.y, 0, 1);
    Math::CreateMatrix(&matrix, &pos, &Vector4f::ONE, &Quaternionf::IDENTITY);

    // setup clipping region so text overlay does not exceed to label size
    pos = transform * m_physics.position();
    m_textOverlay->renderData()->setClipRect(Rectf(pos.x, pos.y, size().x, size().y));
    m_textOverlay->addForRendering(renderer, transform * matrix);

    // call base class
    Widget::addForRendering(renderer, transform);
  }
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

  if (params.contains("text-alignment"))
  {
    Alignment alignment = params.at("text-alignment").toAlignment(&error);
    setTextAlignment(alignment);
  }

  return !error;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets content text. */
void Label::setText(const Text& text)
{
  m_textOverlay->setText(text);
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
/*! Widget override. Sets transparency level. */
void Label::setAlpha(float32 alpha)
{
  // apply to text overlay
  m_textOverlay->setAlpha(alpha);

  // call base class
  Widget::setAlpha(alpha);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets text alignment. */
void Label::setTextAlignment(Alignment alignment)
{
  m_textOverlay->setTextAlignment(alignment);
  m_textOverlay->setAlignment(alignment);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END