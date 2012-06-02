#include "Core/UI/Widget.h"
#include "Core/UI/ScrollableArea.h"
#include <EGETexture.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Widget)
EGE_DEFINE_DELETE_OPERATORS(Widget)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Widget::Widget(Application* app, const String& name, u32 uid, egeObjectDeleteFunc deleteFunc) : Object(app, uid, deleteFunc), 
                                                                                                m_name(name),
                                                                                                m_renderDataInvalid(true), 
                                                                                                m_visible(true),
                                                                                                m_widgetFrame(NULL),
                                                                                                m_parent(NULL),
                                                                                                m_size(100.0f, 100.0f),
                                                                                                m_alignment(ALIGN_TOP_LEFT),
                                                                                                m_globalTransformationMatrixInvalid(true)
{
  ege_connect(&m_physics, transformationChanged, this, Widget::onTransformationChanged);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Widget::~Widget()
{
  ege_disconnect(&m_physics, transformationChanged, this, Widget::onTransformationChanged);

  EGE_DELETE(m_widgetFrame);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool Widget::isValid() const
{
  return ((!isFrameless() && (NULL != m_widgetFrame)) || (isFrameless() && (NULL == m_widgetFrame)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Initializes emitter from dictionary. */
bool Widget::initialize(const Dictionary& params)
{
  if (!isFrameless())
  {
    m_widgetFrame = ege_new WidgetFrame(app());
    if (NULL == m_widgetFrame)
    {
      // error!
      return false;
    }
  }

  bool error = false;
  Vector2f size = params.value("size", "0 0").toVector2f(&error);
  if ((0 >= size.x) || (0 >= size.y))
  {
    // error!
    error = true;
  }
  setSize(size);

  return !error;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates overlay. */
void Widget::update(const Time& time)
{
  // update children
  for (ChildrenDataMap::const_iterator it = m_children.begin(); it != m_children.end(); ++it)
  {
    const ChildData& childData = it->second;

    childData.widget->update(time);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Renders dialog. */
void Widget::addForRendering(Renderer* renderer, const Matrix4f& transform)
{
  if (isVisible())
  {
    // regenerate render data if required
    if (m_renderDataInvalid)
    {
      generateRenderData();

      // reset flag
      m_renderDataInvalid = false;
    }

    // render frame
    if (NULL != m_widgetFrame)
    {
      m_widgetFrame->addForRendering(renderer, transform * m_physics.transformationMatrix());
    }

    // render children
    for (ChildrenDataMap::const_iterator it = m_children.begin(); it != m_children.end(); ++it)
    {
      const ChildData& childData = it->second;

      childData.widget->addForRendering(renderer, transform * m_physics.transformationMatrix());
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
  if (NULL != m_widgetFrame)
  {
    m_widgetFrame->setMaterial(material);
  }

  // invalidate render data
  m_renderDataInvalid = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Pointer event processor. */
void Widget::pointerEvent(PPointerData data)
{
  const Matrix4f& globalMatrix = globalTransformationMatrix();

  Vector4f pos = globalMatrix.translation();

  Rectf rect(pos.x, pos.y, size().x, size().y);

  // map to client area
  //Vector2f pos(data->x() - m_physics.position().x, data->y() - m_physics.position().y);
  
  // check if inside
  if (rect.contains(static_cast<float32>(data->x()), static_cast<float32>(data->y())))
  {
    // pass to children
    for (ChildrenDataMap::const_iterator it = m_children.begin(); it != m_children.end(); ++it)
    {
      const ChildData& childData = it->second;

      // update position into local space of the child
      // NOTE: content's position is in widget's local space
      //PointerData localData(data->action(), data->button(), static_cast<s32>(pos.x), static_cast<s32>(pos.y), data->index());

      childData.widget->pointerEvent(data);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds child. */
EGEResult Widget::addChild(PWidget widget)
{
  EGE_ASSERT(NULL == widget->parent());

  // check if widget with that name exists
  if (m_children.contains(widget->name()))
  {
    // error!
    return EGE_ERROR_ALREADY_EXISTS;
  }

  ChildData childData;
  childData.widget = widget;

  // add to pool
  m_children.insert(widget->name(), childData);

  // set child parent
  widget->m_parent = this;

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes given child. */
void Widget::removeChild(PWidget widget)
{
  // find proper widget
  for (ChildrenDataMap::iterator it = m_children.begin(); it != m_children.end(); ++it)
  {
    if (it->second.widget == widget)
    {
      // remove from pool
      m_children.erase(it);

      // reset parent
      widget->m_parent = NULL;
      return;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes child with a given name. */
void Widget::removeChild(const String& name)
{
  // find proper widget
  for (ChildrenDataMap::iterator it = m_children.begin(); it != m_children.end(); ++it)
  {
    if (it->first == name)
    {
      PWidget widget = it->second.widget;

      // remove from pool
      m_children.erase(it);

      // reset parent
      widget->m_parent = NULL;
      return;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes all children. */
void Widget::removeAllChildren()
{
  // go thru all children
  for (ChildrenDataMap::iterator it = m_children.begin(); it != m_children.end(); ++it)
  {
    PWidget& widget = it->second.widget;

    // reset parent
    widget->m_parent = NULL;
  }

  m_children.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Generates render data. */
void Widget::generateRenderData()
{
  // do nothing
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Determines size of the widget (in pixels). */
Vector2f Widget::size()
{
  //static const float32 hugeSize = 10000.0f;

  //if (!m_sizeValid)
  //{
  //  Vector2f contentSize = this->contentSize();

  //  // make sure widget size is in proper range
  //  Vector2f minFrameSize(0, 0);
  //  if (NULL != m_widgetFrame)
  //  {
  //    minFrameSize = m_widgetFrame->minSize();
  //  }
  //  m_size.x = Math::Bound(contentSize.x, minFrameSize.x, (0.0f == m_maxSize.x) ? hugeSize : m_maxSize.x);
  //  m_size.y = Math::Bound(contentSize.y, minFrameSize.y, (0.0f == m_maxSize.y) ? hugeSize : m_maxSize.y);

  //  // go thru all children and reassign layouts
  //  for (ChildrenDataMap::iterator it = m_children.begin(); it != m_children.end(); ++it)
  //  {
  //    ChildData& data = it->second;

  //    data.widget->setPosition(Vector4f(data.rect.x * m_size.x, data.rect.y * m_size.y, 0));
  //    data.widget->setSize(Vector2f(data.rect.width * m_size.x, data.rect.height * m_size.y));

  //    if (data.widget->m_widgetFrame)
  //    {
  //      data.widget->m_widgetFrame->setSize(Vector2f(data.rect.width * m_size.x, data.rect.height * m_size.y));
  //    }
  //  }

  //  // pass size to widget frame if any
  //  if (m_widgetFrame)
  //  {
  //    m_widgetFrame->setSize(m_size);
  //  }

  //  // set flag
  //  m_sizeValid = true;
  //}

  return m_size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns child with a given name. Optionally, stores child rectangle within parent. */
PWidget Widget::child(const String& name) const
{
  // go thru all children
  for (ChildrenDataMap::const_iterator it = m_children.begin(); it != m_children.end(); ++it)
  {
    // check if found
    if (it->first == name)
    {
      return it->second.widget;
    }
  }

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets name. */
void Widget::setName(const String& name)
{
  m_name = name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Detrmines widget's content size (in pixels). */
Vector2f Widget::contentSize()
{
  Vector2f biggestSize = Vector2f::ZERO;

  // go thru all children
  for (ChildrenDataMap::iterator it = m_children.begin(); it != m_children.end(); ++it)
  {
    const ChildData& data = it->second;

    // determine content size
    Vector2f contentSize = data.widget->contentSize();

    // calculate desired dialog size to match requirements for this content area
    //Vector2f sizeFromContent(contentSize.x / data.rect.width, contentSize.y / data.rect.height);

    // store if bigger than whats in so far
    //biggestSize.x = Math::Max(biggestSize.x, sizeFromContent.x);
    //biggestSize.y = Math::Max(biggestSize.y, sizeFromContent.y);
  }

  return biggestSize;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets size. */
void Widget::setSize(const Vector2f& size)
{
  m_size = size;

  // pass to frame if present
  if (m_widgetFrame)
  {
    m_widgetFrame->setSize(size);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets position. */
void Widget::setPosition(const Vector4f& position)
{
  m_physics.setPosition(position);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets transparency level. */
void Widget::setAlpha(float32 alpha)
{
  // apply to frame if any
  if (NULL != m_widgetFrame)
  {
    m_widgetFrame->renderComponent()->material()->setDiffuseAlpha(alpha);
  }

  // apply to all children
  for (ChildrenDataMap::iterator it = m_children.begin(); it != m_children.end(); ++it)
  {
    ChildData& data = it->second;

    data.widget->setAlpha(alpha);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets widget alignment. */
void Widget::setAlignment(Alignment alignment)
{
  m_alignment = alignment;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when own transformation has been changed. */
void Widget::onTransformationChanged()
{
  // check if not invalid yet
  if (!m_globalTransformationMatrixInvalid)
  {
    // invalidate global transformation
    m_globalTransformationMatrixInvalid = true;

    // notify children
    for (ChildrenDataMap::iterator it = m_children.begin(); it != m_children.end(); ++it)
    {
      it->second.widget->onTransformationChanged();
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns global transformation matrix. */
const Matrix4f& Widget::globalTransformationMatrix() const
{
  // check if matrix is invalid
  if (m_globalTransformationMatrixInvalid)
  {
    if (m_parent)
    {
      m_globalTransformation = m_parent->globalTransformationMatrix() * m_physics.transformationMatrix();
    }
    else
    {
      m_globalTransformation = m_physics.transformationMatrix();
    }

    // validate
    m_globalTransformationMatrixInvalid = false;
  }

  return m_globalTransformation;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END