#include "Core/UI/Dialog.h"
#include "Core/UI/WidgetFactory.h"
#include "EGEEngine.h"
#include "EGEResources.h"
#include "EGEGraphics.h"
#include "EGERenderer.h"
#include "EGEStringUtils.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Dialog)
EGE_DEFINE_DELETE_OPERATORS(Dialog)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const Rectf l_defaultTitleAreaRect = Rectf(0, 0.5f, 1.0f, 0.2f);
static const Rectf l_defaultTextAreaRect  = Rectf(0, 0.2f, 1.0f, 0.8f);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Dialog::Dialog(Engine& engine, const String& name, egeObjectDeleteFunc deleteFunc) 
: Widget(engine, name, EGE_OBJECT_UID_UI_DIALOG, deleteFunc)
{
  // create vertex declaration
  VertexDeclaration declaration;
  if (declaration.addElement(NVertexBuffer::VES_POSITION_XY) && declaration.addElement(NVertexBuffer::VES_TEXTURE_UV))
  {
    // create tail render data
    m_tailRenderData  = ege_new RenderComponent(engine, "dialog-tail" + name, declaration, EGEGraphics::RP_MAIN, EGEGraphics::RPT_TRIANGLE_STRIPS, 
                                                NVertexBuffer::UT_STATIC_WRITE);

    PResourceFont fontResource = engine.resourceManager()->resource(RESOURCE_NAME_FONT, "debug-font");
    if (fontResource)
    {
      m_titleFont = fontResource->font();
      m_textFont  = fontResource->font();

      // title label
      PLabel label = engine.graphics()->widgetFactory()->createWidget("label", "title");
      if (label)
      {
        label->setFont(m_titleFont);
        addChild(label);
      }

      // text label
      label = engine.graphics()->widgetFactory()->createWidget("label", "text");
      if (label)
      {
        label->setFont(m_textFont);
        addChild(label);
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Dialog::~Dialog()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PWidget Dialog::Create(Engine& engine, const String& name)
{
  return ege_new Dialog(engine, name);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Dialog::isValid() const
{
  return Widget::isValid() && (NULL != child("title")) && (NULL != child("text"));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Dialog::update(const Time& time)
{
  // call base class
  Widget::update(time);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Dialog::addForRendering(IRenderer* renderer, const Matrix4f& transform)
{
  if (isVisible())
  {
    // tail
    renderer->addForRendering(m_tailRenderData, transform * m_physics.transformationMatrix());

    // call base class
    Widget::addForRendering(renderer, transform);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Dialog::generateRenderData()
{
  // TAGE - need to find better way of doing this...
  Vector2f textureSize(m_tailRenderData->material()->pass(0)->texture(0)->width() * 1.0f, m_tailRenderData->material()->pass(0)->texture(0)->height() * 1.0f);
  textureSize.set(1.0f / textureSize.x, 1.0f / textureSize.y);

  // set buffer size
  if (m_tailRenderData->vertexBuffer()->setSize(4))
  {
    float32* data = reinterpret_cast<float32*>(m_tailRenderData->vertexBuffer()->lock(0, 4));
    if (NULL != data)
    {
      *data++ = m_tailOffset.x;
      *data++ = m_tailOffset.y + m_tailRect.height;
      *data++ = m_tailRect.x * textureSize.x;
      *data++ = (m_tailRect.y + m_tailRect.height) * textureSize.y;

      *data++ = m_tailOffset.x + m_tailRect.width;
      *data++ = m_tailOffset.y + m_tailRect.height;
      *data++ = (m_tailRect.x + m_tailRect.width) * textureSize.x;
      *data++ = (m_tailRect.y + m_tailRect.height) * textureSize.y;

      *data++ = m_tailOffset.x;
      *data++ = m_tailOffset.y;
      *data++ = m_tailRect.x * textureSize.x;
      *data++ = m_tailRect.y * textureSize.y;

      *data++ = m_tailOffset.x + m_tailRect.width;
      *data++ = m_tailOffset.y;
      *data++ = (m_tailRect.x + m_tailRect.width) * textureSize.x;
      *data++ = m_tailRect.y * textureSize.y;
    }
    m_tailRenderData->vertexBuffer()->unlock(data - 1);
  }

  m_tailRenderData->setPriority(m_widgetFrame->renderComponent()->priority() + 1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
bool Dialog::isFrameless() const
{
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Dialog::initialize(const Dictionary& params)
{
  // initialize base
  bool error = !Widget::initialize(params);
  
  // check if material name is defined
  if (params.contains("material"))
  {
    PResourceMaterial materialResource = engine().resourceManager()->materialResource(params.at("material"));
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

      // set tail render data material
      m_tailRenderData->setMaterial(material);
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

  PLabel titlelabel = child("title");
  PLabel textLabel  = child("text");

  // TITLE AREA
  if (params.contains("title-rect"))
  {
    Rectf rect = StringUtils::ToRectf(params.at("title-rect"), &error);
    
    titlelabel->setPosition(Vector4f(rect.x, rect.y, 0));
    titlelabel->setSize(Vector2f(rect.width, rect.height));
  }

  if (params.contains("title-font"))
  {
    PResourceFont fontResource = engine().resourceManager()->resource(RESOURCE_NAME_FONT, params.at("title-font"));
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

  if (params.contains("title-alignment"))
  {
    Alignment alignment = StringUtils::ToAlignment(params.at("title-alignment"), &error);
    titlelabel->setTextAlignment(alignment);
  }

  // TEXT AREA
  if (params.contains("text-rect"))
  {
    Rectf rect = StringUtils::ToRectf(params.at("text-rect"), &error);

    textLabel->setPosition(Vector4f(rect.x, rect.y, 0));
    textLabel->setSize(Vector2f(rect.width, rect.height));
  }

  if (params.contains("text-font"))
  {
    PResourceFont fontResource = engine().resourceManager()->resource(RESOURCE_NAME_FONT, params.at("text-font"));
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

  if (params.contains("text-alignment"))
  {
    Alignment alignment = StringUtils::ToAlignment(params.at("text-alignment"), &error);
    textLabel->setTextAlignment(alignment);
  }

  // TAIL
  if (params.contains("tail-rect"))
  {
    m_tailRect = StringUtils::ToRecti(params.at("tail-rect"), &error);
  }

  if (params.contains("tail-offset"))
  {
    m_tailOffset = StringUtils::ToVector2f(params.at("tail-offset"), &error);
  }

  return !error;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
void Dialog::setAlpha(float32 alpha)
{
  // call base class
  Widget::setAlpha(alpha);

  // propagate to tail material
  m_tailRenderData->material()->setDiffuseAlpha(alpha);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END