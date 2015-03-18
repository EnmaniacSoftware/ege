#include "Core/UI/Label.h"
#include "EGEEngine.h"
#include "EGEResourceManager.h"
#include "EGEResources.h"
#include "EGEStringUtils.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Label)
EGE_DEFINE_DELETE_OPERATORS(Label)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Label::Label(Engine& engine, const String& name, egeObjectDeleteFunc deleteFunc) 
: Widget(engine, name, EGE_OBJECT_UID_UI_LABEL, deleteFunc)
{
  // set default font
  PResourceFont fontResource = engine.resourceManager()->resource(RESOURCE_NAME_FONT, "debug-font");
  if (fontResource)
  {
    // add text overlays
    m_textOverlay = ege_new TextOverlay(engine, "text");

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
PWidget Label::Create(Engine& engine, const String& name)
{
  return ege_new Label(engine, name);
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

    textRect = Math::Align(textRect, labelRect, ALIGN_TOP_LEFT, m_textOverlay->textAlignment());

    Matrix4f matrix = physics().transformationMatrix();
    Vector2f pos(matrix.translationX(), matrix.translationY());
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
    PResourceFont fontResource = engine().resourceManager()->resource(RESOURCE_NAME_FONT, params.value("font"));
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
    Alignment alignment = StringUtils::ToAlignment(params.value("text-alignment"), &error);
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