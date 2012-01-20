#include "Core/UI/Widget.h"
#include "Core/UI/ScrollableArea.h"
#include <EGETexture.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Widget)
EGE_DEFINE_DELETE_OPERATORS(Widget)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Widget::Widget(Application* app, const String& name, u32 uid, egeObjectDeleteFunc deleteFunc) : Object(app, uid, deleteFunc), 
                                                                                                m_name(name),
                                                                                                m_renderDataInvalid(true), 
                                                                                                m_visible(true),
                                                                                                m_maxSize(Vector2i::ZERO),
                                                                                                m_widgetFrame(NULL),
                                                                                                m_sizeValid(false),
                                                                                                m_parent(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Widget::~Widget()
{
  m_renderData = NULL;

  EGE_DELETE(m_widgetFrame);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool Widget::isValid() const
{
  return (NULL != m_renderData) && ((!isFrameless() && (NULL != m_widgetFrame)) || (isFrameless() && (NULL == m_widgetFrame)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates overlay. */
void Widget::update(const Time& time)
{
  // update content
  for (ContentAreaDataMap::const_iterator it = m_contentAreas.begin(); it != m_contentAreas.end(); ++it)
  {
    const ContentAreaData& data = it->second;

    data.area->update(time);
  }

  // update children
  for (ChildrenMap::const_iterator it = m_children.begin(); it != m_children.end(); ++it)
  {
    const PWidget& child = it->second;

    child->update(time);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Renders dialog. */
void Widget::addForRendering(Renderer* renderer, const Matrix4f& transform)
{
  if (visible())
  {
    // regenerate render data if required
    if (m_renderDataInvalid)
    {
      generateRenderData();
    }

    // update size if necessary to make sure frame has it correct before rendering
    //if (m_sizeValid)
    {
      if (NULL != m_widgetFrame)
      {
        m_widgetFrame->setSize(Vector2f(size().x, size().y));
      }
    }

    // render frame
    if (NULL != m_widgetFrame)
    {
      m_widgetFrame->addForRendering(renderer, transform.multiply(m_physics.transformationMatrix()));
    }

    // render content
    for (ContentAreaDataMap::const_iterator it = m_contentAreas.begin(); it != m_contentAreas.end(); ++it)
    {
      const ContentAreaData& data = it->second;

      data.area->addForRendering(renderer, transform.multiply(m_physics.transformationMatrix()));
    }

    // render children
    for (ChildrenMap::const_iterator it = m_children.begin(); it != m_children.end(); ++it)
    {
      const PWidget& child = it->second;

      child->addForRendering(renderer, transform * m_physics.transformationMatrix());
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets visibility. */
void Widget::setVisible(bool set)
{
  if (m_visible != set)
  {
    m_visible = set;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets material. */
void Widget::setMaterial(const PMaterial& material)
{
  EGE_ASSERT(m_renderData);
  m_renderData->setMaterial(material);

  if (NULL != m_widgetFrame)
  {
    m_widgetFrame->setMaterial(material);
  }

  // invalidate render data
  m_renderDataInvalid = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets max size. */
void Widget::setMaxSize(const Vector2i& size)
{
  m_maxSize = size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Pointer event processor. */
void Widget::pointerEvent(PPointerData data)
{
  // map to client area
  Vector2f pos(data->x() - m_physics.position().x, data->y() - m_physics.position().y);
  
  // check if inside
  if ((0 <= pos.x) && (pos.x < size().x) && (0 <= pos.y) && (pos.y < size().y))
  {
    // go thru all content areas
    for (ContentAreaDataMap::const_iterator it = m_contentAreas.begin(); it != m_contentAreas.end(); ++it)
    {
      const ContentAreaData& contentData = it->second;

      // update position into local space of the content
      // NOTE: content's position is in widget's local space
      PointerData localData(data->action(), data->button(), static_cast<s32>(pos.x), static_cast<s32>(pos.y), data->index());
    
      contentData.area->pointerEvent(localData);
    }

    // pass to children
    for (ChildrenMap::const_iterator it = m_children.begin(); it != m_children.end(); ++it)
    {
      const PWidget& child = it->second;

      // update position into local space of the child
      // NOTE: content's position is in widget's local space
      PointerData localData(data->action(), data->button(), static_cast<s32>(pos.x), static_cast<s32>(pos.y), data->index());

      child->pointerEvent(localData);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Determines size of the content only (in pixels). */
Vector2i Widget::contentSize() const
{
  Vector2i size(0, 0);

  // go thru all content areas
  for (ContentAreaDataMap::const_iterator it = m_contentAreas.begin(); it != m_contentAreas.end(); ++it)
  {
    const ContentAreaData& data = it->second;

    size.x += static_cast<s32>(data.area->contentSize().x);
    size.y += static_cast<s32>(data.area->contentSize().y);
  }

  return size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Add content area. */
EGEResult Widget::addContentArea(const String& name, const Rectf& rect, bool verticalScroll, bool horizontalScroll)
{
  ContentAreaData contentArea;

  // check if already exists
  if (m_contentAreas.contains(name))
  {
    // error!
    return EGE_ERROR_ALREADY_EXISTS;
  }
 
  // allocate scrollable area
  contentArea.area = ege_new ScrollableArea(app(), name);
  contentArea.rect = rect;
  if (NULL == contentArea.area)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // setup scrollable area
  contentArea.area->setScrollbarsEnabled(true);
  contentArea.area->setDirection(((horizontalScroll) ? ScrollableArea::DIRECTION_HORIZONTAL : ScrollableArea::DIRECTION_NONE) | 
                                 ((verticalScroll) ? ScrollableArea::DIRECTION_VERTICAL : ScrollableArea::DIRECTION_NONE));

  // add to pool
  m_contentAreas.insert(name, contentArea);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds child. */
EGEResult Widget::addChild(PWidget widget)
{
  // check if widget with that name exists
  if (m_children.contains(widget->name()))
  {
    // error!
    return EGE_ERROR_ALREADY_EXISTS;
  }

  // add to pool
  m_children.insert(widget->name(), widget);
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes given child. */
void Widget::removeChild(PWidget widget)
{
  m_children.removeByValue(widget);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes child with a given name. */
void Widget::removeChild(const String& name)
{
  m_children.removeByKey(name);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes all children. */
void Widget::removeAllChildren()
{
  m_children.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Generates render data. */
void Widget::generateRenderData()
{
  // do nothing
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Determines size of the dialog (in pixels). */
Vector2i Widget::size()
{
  if (!m_sizeValid)
  {
    Vector2f biggestSize = Vector2f::ZERO;

    // go thru all content areas
    for (ContentAreaDataMap::const_iterator it = m_contentAreas.begin(); it != m_contentAreas.end(); ++it)
    {
      const ContentAreaData& data = it->second;

      // determine content size
      Vector2f contentSize = data.area->contentSize();

      // calculate desired dialog size to match requirements for this content area
      Vector2f sizeFromContent(contentSize.x / data.rect.width, contentSize.y / data.rect.height);

      // store if bigger than whats in so far
      biggestSize.x = Math::Max(biggestSize.x, sizeFromContent.x);
      biggestSize.y = Math::Max(biggestSize.y, sizeFromContent.y);
    }

    // make sure dialog size is in proper range
    Vector2f minFrameSize(0, 0);
    if (NULL != m_widgetFrame)
    {
      minFrameSize = m_widgetFrame->minSize();
    }
    m_size.x = Math::Bound(static_cast<s32>(biggestSize.x), static_cast<s32>(minFrameSize.x), (0 == m_maxSize.x) ? Math::MAX_S32 : m_maxSize.x);
    m_size.y = Math::Bound(static_cast<s32>(biggestSize.y), static_cast<s32>(minFrameSize.y), (0 == m_maxSize.y) ? Math::MAX_S32 : m_maxSize.y);

    // go thru all content and reassign layouts
    for (ContentAreaDataMap::iterator it = m_contentAreas.begin(); it != m_contentAreas.end(); ++it)
    {
      ContentAreaData& data = it->second;

      data.area->physics().setPosition(Vector4f(data.rect.x * m_size.x, data.rect.y * m_size.y, 0));
      data.area->physics().setScale(Vector4f(data.rect.width * m_size.x, data.rect.height * m_size.y, 0));
    }

    // pass size to widget frame
    m_widgetFrame->setSize(Vector2f(static_cast<float32>(m_size.x), static_cast<float32>(m_size.y)));

    // set flag
    m_sizeValid = true;
  }

  return m_size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Initializes the object. This method is to be called from derived objects. */
void Widget::initialize()
{
  if (!isFrameless())
  {
    m_widgetFrame = ege_new WidgetFrame(app());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
