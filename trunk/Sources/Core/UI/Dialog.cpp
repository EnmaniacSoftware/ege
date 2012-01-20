#include "Core/UI/Dialog.h"
#include "Core/UI/ScrollableArea.h"
#include <EGEApplication.h>
#include <EGETexture.h>
#include <EGEResources.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Dialog)
EGE_DEFINE_DELETE_OPERATORS(Dialog)
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
    // add text and title overlays
    m_titleOverlay = ege_new TextOverlay(app, "title");
    m_textOverlay  = ege_new TextOverlay(app, "text");

    if ((NULL != m_titleOverlay) && (NULL != m_textOverlay))
    {
      m_titleOverlay->setFont(fontResource->font());
      m_textOverlay->setFont(fontResource->font());

      m_titleOverlay->physics()->setPosition(Vector4f(0, 0, 0));
      m_textOverlay->physics()->setPosition(Vector4f(0, 0, 0));

      ContentAreaData dummy;

      ContentAreaData& contentArea = m_contentAreas.value("title", dummy);
      if (NULL != contentArea.area)
      {
        contentArea.area->addObject(m_titleOverlay);
      }

      contentArea = m_contentAreas.value("text", dummy);
      if (NULL != contentArea.area)
      {
        contentArea.area->addObject(m_textOverlay);
      }
    }
  }  

  // initialize
  initialize();
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
  return Widget::isValid() && m_contentAreas.contains("title") && m_contentAreas.contains("text") && (NULL != m_textOverlay) && (NULL != m_titleOverlay);
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
  if (NULL != m_titleOverlay)
  {
    m_titleOverlay->setText(title);
  }

  // invalidate size
  m_sizeValid = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets content text. */
void Dialog::setText(const Text& text)
{
  if (NULL != m_textOverlay)
  {
    m_textOverlay->setText(text);
  }

  // invalidate size
  m_sizeValid = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Widget override. Returns TRUE if widget is frameless. */
bool Dialog::isFrameless() const
{
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
