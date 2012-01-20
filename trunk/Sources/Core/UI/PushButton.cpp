#include "Core/UI/PushButton.h"
#include <EGETexture.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(PushButton)
EGE_DEFINE_DELETE_OPERATORS(PushButton)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PushButton::PushButton(Application* app, const String& name, egeObjectDeleteFunc deleteFunc) : Widget(app, name, EGE_OBJECT_UID_UI_PUSH_BUTTON, deleteFunc)
{
  m_renderData  = ege_new RenderComponent(app, "push-button-" + name, EGEGraphics::RP_MAIN, EGEGraphics::RPT_TRIANGLES, EGEVertexBuffer::UT_STATIC_WRITE);
  if (m_renderData)
  {
    m_renderData->indexBuffer()->setIndexSize(EGEIndexBuffer::IS_8BIT);
    if (!m_renderData->vertexBuffer()->setSemantics(EGEVertexBuffer::ST_V2_T2))
    {
      // error!
      m_renderData = NULL;
    }
  }

  // initialize
  initialize();
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
  // call base class
  Widget::addForRendering(renderer, transform);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Determines size of the dialog (in pixels). */
Vector2i PushButton::size()
{
  //if (!m_sizeValid)
  //{
  //  Vector2f biggestSize = Vector2f::ZERO;

  //  // go thru all content areas
  //  for (ContentAreaDataMap::const_iterator it = m_contentAreas.begin(); it != m_contentAreas.end(); ++it)
  //  {
  //    const ContentAreaData& data = it->second;

  //    // determine content size
  //    Vector2f contentSize = data.area->contentSize();

  //    // calculate desired dialog size to match requirements for this content area
  //    Vector2f sizeFromContent(contentSize.x / data.rect.width, contentSize.y / data.rect.height);

  //    // store if bigger than whats in so far
  //    biggestSize.x = Math::Max(biggestSize.x, sizeFromContent.x);
  //    biggestSize.y = Math::Max(biggestSize.y, sizeFromContent.y);
  //  }

  //  // make sure dialog size is in proper range
  //  m_size.x = Math::Bound(static_cast<s32>(biggestSize.x), static_cast<s32>(m_widgetFrame->minSize().x), (0 == m_maxSize.x) ? Math::MAX_S32 : m_maxSize.x);
  //  m_size.y = Math::Bound(static_cast<s32>(biggestSize.y), static_cast<s32>(m_widgetFrame->minSize().y), (0 == m_maxSize.y) ? Math::MAX_S32 : m_maxSize.y);

  //  // go thru all content and reassign layouts
  //  for (ContentAreaDataMap::iterator it = m_contentAreas.begin(); it != m_contentAreas.end(); ++it)
  //  {
  //    ContentAreaData& data = it->second;

  //    data.area->physics().setPosition(Vector4f(data.rect.x * m_size.x, data.rect.y * m_size.y, 0));
  //    data.area->physics().setScale(Vector4f(data.rect.width * m_size.x, data.rect.height * m_size.y, 0));
  //  }

  //  // pass size to widget frame
  //  m_widgetFrame->setSize(Vector2f(static_cast<float32>(m_size.x), static_cast<float32>(m_size.y)));

  //  // set flag
  //  m_sizeValid = true;
  //}

  return m_size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Pointer event processor. */
void PushButton::pointerEvent(PPointerData data)
{
  // map to client area
  Vector2f pos(data->x() - m_physics.position().x, data->y() - m_physics.position().y);
  
  // check if inside
  if ((0 <= pos.x) && (pos.x < size().x) && (0 <= pos.y) && (pos.y < size().y))
  {
  }
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
