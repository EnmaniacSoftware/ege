#include "Core/UI/Label.h"
#include "EGEApplication.h"
#include "EGEResources.h"
#include "EGEStringUtils.h"

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
PWidget Label::Create(Application* app, const String& name)
{
  return ege_new Label(app, name);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Label::isValid() const
{
  return Widget::isValid() && (NULL != m_textOverlay);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Label::update(const Time& time)
{
  // call base class
  Widget::update(time);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Label::addForRendering(IRenderer* renderer, const Matrix4f& transform)
{
  if (isVisible())
  {
    // TAGE - probably not right, what if scale is non identity ?

    // align text overlay within label
    Vector2f textSize   = m_textOverlay->size();
    Vector2f labelSize  = size();

    Rectf textRect(0, 0, textSize.x, textSize.y);
    Rectf labelRect(0, 0, labelSize.x, labelSize.y);

    Math::Align(&textRect, &labelRect, ALIGN_TOP_LEFT, m_textOverlay->textAlignment());

    Matrix4f matrix = physics().transformationMatrix();
    Vector4f pos = matrix.translation();
    pos.x += textRect.x;
    pos.y += textRect.y;
    matrix.setTranslation(pos.x, pos.y, 0);

   // m_textOverlay->renderData()->setClipRect(Rectf(pos.x, pos.y, size().x, size().y));
    m_textOverlay->addForRendering(renderer, transform * matrix);

    // call base class
    Widget::addForRendering(renderer, transform);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Label::pointerEvent(PPointerData data)
{
  // call base class
  Widget::pointerEvent(data);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Label::isFrameless() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
    Alignment alignment = StringUtils::ToAlignment(params.at("text-alignment"), &error);
    setTextAlignment(alignment);
  }

  return !error;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Label::setText(const Text& text)
{
  m_textOverlay->setText(text);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Label::setFont(PFont font)
{
  m_textOverlay->setFont(font);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector2f Label::contentSize()
{
  return m_textOverlay->size();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Label::setAlpha(float32 alpha)
{
  // apply to text overlay
  m_textOverlay->setAlpha(alpha);

  // call base class
  Widget::setAlpha(alpha);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Label::setTextAlignment(Alignment alignment)
{
  m_textOverlay->setTextAlignment(alignment);
  //m_textOverlay->setAlignment(alignment);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END