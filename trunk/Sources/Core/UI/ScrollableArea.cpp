#include "Core/UI/ScrollableArea.h"
#include <EGEOverlay.h>
#include <EGEMath.h>
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ScrollableArea)
EGE_DEFINE_DELETE_OPERATORS(ScrollableArea)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ScrollableArea::ScrollableArea(Application* app, const String& name, egeObjectDeleteFunc deleteFunc) 
: Widget(app, name, EGE_OBJECT_UID_UI_SCROLLABLE_AREA, deleteFunc),
  m_state(STATE_IDLE),
  m_dampingCoefficient(0.95f, 0.95f),
  m_throwCoefficient(10.0f, 10.0f),
  m_baseReturnCoefficient(3.0f, 3.0f),
  m_direction(DIRECTION_BOTH),
  m_scrollbarsEnabled(true),
  m_scrollbarsFadeDuration(0.5f),
  m_scrollbarsState(SS_HIDDEN),
  m_dirtyContent(false)
{
  PMaterial material = ege_new Material(app);
  if (material)
  {
    if (NULL != material->addPass(NULL))
    {
      material->setSrcBlendFactor(EGEGraphics::BF_SRC_ALPHA);
      material->setDstBlendFactor(EGEGraphics::BF_ONE_MINUS_SRC_ALPHA);
      material->setDiffuseColor(Color::NONE);
    
      m_renderData = ege_new RenderComponent(app, "scrollable-area-scrollbar", EGEGraphics::RP_MAIN + 20, EGEGraphics::RPT_TRIANGLES);
      if (m_renderData)
      {
        m_renderData->setMaterial(material);
        if (!m_renderData->vertexBuffer()->setSemantics(EGEVertexBuffer::ST_V2))
        {
          // error!
          m_renderData = NULL;
        }
      }
    }
  }

  ege_connect(&m_physics, transformationChanged, this, ScrollableArea::transformationChanged);

  // initialize
  initialize();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ScrollableArea::~ScrollableArea()
{
  ege_disconnect(&m_physics, transformationChanged, this, ScrollableArea::transformationChanged);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of widget. This method is a registration method for factory. */
PWidget ScrollableArea::Create(Application* app, const String& name)
{
  return ege_new ScrollableArea(app, name);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates object. */
void ScrollableArea::update(const Time& time)
{
  static const float32 cutOffValue = 0.1f;

  // check if content needs to be recalculated
  if (m_dirtyContent)
  {
    // recalculate content size
    recaluclateContentSize();

    // reset flag
    m_dirtyContent = false;
  }

  // update objects
  for (ObjectsList::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    switch ((*it)->uid())
    {
      case EGE_OBJECT_UID_OVERLAY_TEXT:
        {
          TextOverlay* overlay = ege_cast<TextOverlay*>(*it);

          overlay->update(time);
        }
        break;
    }
  }

  // upate scrolling
  Vector2f displacement = m_lastPointerPosition - m_currentPointerPosition;

  // limit movement according to direction
  if (0 == (m_direction & DIRECTION_VERTICAL))
  {
    displacement.y = 0.0f;
  }

  if (0 == (m_direction & DIRECTION_HORIZONTAL))
  {
    displacement.x = 0.0f;
  }

  // check if dragging
  if (STATE_DRAGGED == m_state)
  {
    // while content is being dragged scroll immediately to final position
    m_scrollOffset += displacement;

    // calculate momentum based on durrent displacement
    m_scrollVelocity = displacement * m_throwCoefficient;
  }
  else
  {
    // check if outside boundary
    if (0 > m_scrollOffset.x)
    {
      m_scrollVelocity.x = -m_scrollOffset.x * m_baseReturnCoefficient.x;
    }
    else if (cutOffValue < (m_scrollOffset.x - m_scrollRange.x))
    {
      m_scrollVelocity.x = -(m_scrollOffset.x - m_scrollRange.x) * m_baseReturnCoefficient.x;
    }

    if (0 > m_scrollOffset.y)
    {
      m_scrollVelocity.y = -m_scrollOffset.y * m_baseReturnCoefficient.y;
    }
    else if (cutOffValue < (m_scrollOffset.y - m_scrollRange.y))
    {
      m_scrollVelocity.y = -(m_scrollOffset.y - m_scrollRange.y) * m_baseReturnCoefficient.y;
    }

    // update position
    m_scrollOffset += m_scrollVelocity * time.seconds();

    // dampen velocity
    m_scrollVelocity.x *= m_dampingCoefficient.x;
    m_scrollVelocity.y *= m_dampingCoefficient.y;

    if (cutOffValue > Math::Abs(m_scrollVelocity.x))
    {
      m_scrollVelocity.x = 0.0f;
    }

    if (cutOffValue > Math::Abs(m_scrollVelocity.y))
    {
      m_scrollVelocity.y = 0.0f;
    }
  }
  
  //EGE_PRINT("Off: %.2f Rng: %.2f Vel: %.2f", m_scrollOffset.y, m_scrollRange.y, m_scrollVelocity.y);

  // update cache
  m_lastPointerPosition = m_currentPointerPosition;

  // process scrollbars if necessary
  if (m_scrollbarsEnabled)
  {
    // check if scrollbars should be hidden
    if ((Vector2f::ZERO == m_scrollVelocity) && (SS_SHOWN == m_scrollbarsState) && (STATE_DRAGGED != m_state))
    {
      // hide scrollbars
      hidesScrollbars();
    }

    // check if update is required
    if (SS_HIDDEN != m_scrollbarsState)
    {
      // update scrollbars
      updateScrollbars(time);
    }
  }

  // call base class
  Widget::update(time);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Pointer event processor. */
void ScrollableArea::pointerEvent(PPointerData data)
{
  if (EGEInput::ACTION_BUTTON_DOWN == data->action())
  {
    // map to client area
    Vector2f pos(data->x() - m_physics.position().x, data->y() - m_physics.position().y);
  
    // check if inside
    if ((0 <= pos.x) && (pos.x < m_physics.scale().x) && (0 <= pos.y) && (pos.y < m_physics.scale().y))
    {
      // store current pointer position
      m_currentPointerPosition.x = static_cast<float32>(data->x());
      m_currentPointerPosition.y = static_cast<float32>(data->y());

      // initially last and current are the same
      m_lastPointerPosition = m_currentPointerPosition;

      // change state
      m_state = STATE_DRAGGED;

      // show scrollbars if necessary
      if (m_scrollbarsEnabled)
      {
        showScrollbars();
      }
    }
  }
  else if (EGEInput::ACTION_BUTTON_UP == data->action())
  {
    // change state
    m_state = STATE_IDLE;
  }
  else if ((EGEInput::ACTION_MOVE == data->action()) && (STATE_DRAGGED == m_state))
  {
    // store current pointer position
    m_currentPointerPosition.x = static_cast<float32>(data->x());
    m_currentPointerPosition.y = static_cast<float32>(data->y());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Renders object. */
void ScrollableArea::addForRendering(Renderer* renderer, const Matrix4f& transform)
{
  Vector4f pos;

  // create content objects matrix affected by current scroll offset
  Matrix4f contentMatrix = Matrix4f::IDENTITY;
  pos = m_physics.position();
  pos.x -= m_scrollOffset.x;
  pos.y -= m_scrollOffset.y;
  Math::CreateMatrix(&contentMatrix, &pos, &Vector4f::ONE, &Quaternionf::IDENTITY);
  contentMatrix = transform.multiply(contentMatrix);

  // create matrix describing final position
  Matrix4f matrix = Matrix4f::IDENTITY;
  pos = m_physics.position();
  Math::CreateMatrix(&matrix, &pos, &Vector4f::ONE, &Quaternionf::IDENTITY);
  matrix = transform.multiply(matrix);

  // cache position
  pos = matrix.translation();

  // render all objects
  for (ObjectsList::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    switch ((*it)->uid())
    {
      case EGE_OBJECT_UID_OVERLAY_TEXT:
        {
          TextOverlay* overlay = ege_cast<TextOverlay*>(*it);

          overlay->renderData()->setClipRect(Rectf(pos.x, pos.y, m_physics.scale().x, m_physics.scale().y));
          renderer->addForRendering(overlay->renderData(), contentMatrix * overlay->physics()->transformationMatrix());
        }
        break;
    }
  }

  // render scrollbars if required
  if (m_scrollbarsEnabled && (SS_HIDDEN != m_scrollbarsState))
  {
    static const float32 scrollbarSize = 2.0f;

    // update render data
    float32* data = reinterpret_cast<float32*>(m_renderData->vertexBuffer()->lock(0, 12));

    // vertical
    // NOTE: if there is need for scrolling do not show scrollbar
    if ((DIRECTION_VERTICAL & m_direction) && (0 < m_scrollRange.y))
    {
      const float32 pageSize = Math::Ceil(m_physics.scale().y * (m_physics.scale().y / m_contentSize.y));

      Vector2f topLeft(m_physics.position().x + m_physics.scale().x - scrollbarSize, 
                       m_physics.position().y + (m_physics.scale().y - pageSize) * (m_scrollOffset.y / m_scrollRange.y));

      Vector2f bottomRight(m_physics.position().x + m_physics.scale().x, 
                           m_physics.position().y + (m_physics.scale().y - pageSize) * (m_scrollOffset.y / m_scrollRange.y) + pageSize);

      // top left
      *data++ = topLeft.x;
      *data++ = topLeft.y;

      // bottom left
      *data++ = topLeft.x;
      *data++ = bottomRight.y;

      // bottom right
      *data++ = bottomRight.x;
      *data++ = bottomRight.y;

      // top left
      *data++ = topLeft.x;
      *data++ = topLeft.y;

      // bottom right
      *data++ = bottomRight.x;
      *data++ = bottomRight.y;

      // top right
      *data++ = bottomRight.x;
      *data++ = topLeft.y;
    }
    else
    {
      // reset
      *data++ = 0;
      *data++ = 0;
      *data++ = 0;
      *data++ = 0;
      *data++ = 0;
      *data++ = 0;
      *data++ = 0;
      *data++ = 0;
      *data++ = 0;
      *data++ = 0;
      *data++ = 0;
      *data++ = 0;
    }

    // horizontal
    // NOTE: if there is need for scrolling do not show scrollbar
    if ((DIRECTION_HORIZONTAL & m_direction) && (0 < m_scrollRange.x))
    {
      const float32 pageSize = Math::Ceil(m_physics.scale().x * (m_physics.scale().x / m_contentSize.x));

      Vector2f topLeft(m_physics.position().x + (m_physics.scale().x - pageSize) * (m_scrollOffset.x / m_scrollRange.x), 
                       m_physics.position().y + m_physics.scale().y - scrollbarSize);

      Vector2f bottomRight(m_physics.position().x + (m_physics.scale().x - pageSize) * (m_scrollOffset.x / m_scrollRange.x) + pageSize, 
                           m_physics.position().y + m_physics.scale().y);

      // top left
      *data++ = topLeft.x;
      *data++ = topLeft.y;

      // bottom left
      *data++ = topLeft.x;
      *data++ = bottomRight.y;

      // bottom right
      *data++ = bottomRight.x;
      *data++ = bottomRight.y;

      // top left
      *data++ = topLeft.x;
      *data++ = topLeft.y;

      // bottom right
      *data++ = bottomRight.x;
      *data++ = bottomRight.y;

      // top right
      *data++ = bottomRight.x;
      *data++ = topLeft.y;
    }
    else
    {
      // reset horizontal
      *data++ = 0;
      *data++ = 0;
      *data++ = 0;
      *data++ = 0;
      *data++ = 0;
      *data++ = 0;
      *data++ = 0;
      *data++ = 0;
      *data++ = 0;
      *data++ = 0;
      *data++ = 0;
      *data++ = 0;
    }

    m_renderData->vertexBuffer()->unlock();

    m_renderData->setClipRect(Rectf(pos.x, pos.y, m_physics.scale().x, m_physics.scale().y));
    renderer->addForRendering(m_renderData, transform);
  }

  // call base class
  Widget::addForRendering(renderer, transform);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds object. */
EGEResult ScrollableArea::addObject(PObject object)
{
  EGEResult result = EGE_ERROR_NOT_SUPPORTED;

  // process according to object id
  switch (object->uid())
  {
    case EGE_OBJECT_UID_OVERLAY_TEXT:

      // add to pool
      m_objects.push_back(object);

      // invalidate content
      m_dirtyContent = true;
      break;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Sets scroll direction. */
void ScrollableArea::setDirection(Direction direction)
{
  m_direction = direction;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Sets scroll damping coefficient. */
void ScrollableArea::setDampingCoefficient(const Vector2f& coefficient)
{
  m_dampingCoefficient = coefficient;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets scroll throw coefficient. */
void ScrollableArea::setThrowCoefficient(const Vector2f& coefficient)
{
  m_throwCoefficient = coefficient;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets scroll base return coefficient. */
void ScrollableArea::setBaseReturnCoefficient(const Vector2f& coefficient)
{
  m_baseReturnCoefficient = coefficient;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Shows scrollbars. */
void ScrollableArea::showScrollbars()
{
  // process according to state
  switch (m_scrollbarsState)
  {
    case SS_HIDDEN:
        
      // reset time
      m_scrollbarsStateTime = 0.0f;

      // set state
      m_scrollbarsState = SS_FADING_IN;
      break;

    case SS_FADING_IN:
    case SS_SHOWN:

      // do nothing
      break;

    case SS_FADING_OUT:

      // set time
      m_scrollbarsStateTime = m_scrollbarsFadeDuration - m_scrollbarsStateTime;

      // set state
      m_scrollbarsState = SS_FADING_IN;
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Hide scrollbars. */
void ScrollableArea::hidesScrollbars()
{
  // process according to state
  switch (m_scrollbarsState)
  {
    case SS_SHOWN:
        
      // reset time
      m_scrollbarsStateTime = 0.0f;

      // set state
      m_scrollbarsState = SS_FADING_OUT;
      break;

    case SS_FADING_OUT:
    case SS_HIDDEN:

      // do nothing
      break;

    case SS_FADING_IN:

      // set time
      m_scrollbarsStateTime = m_scrollbarsFadeDuration - m_scrollbarsStateTime;

      // set state
      m_scrollbarsState = SS_FADING_OUT;
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates scrollbars. */
void ScrollableArea::updateScrollbars(const Time& time)
{
  // update state time
  m_scrollbarsStateTime += time;

  // process according to state
  switch (m_scrollbarsState)
  {
    case SS_FADING_OUT:

      // check if done
      if (m_scrollbarsStateTime > m_scrollbarsFadeDuration)
      {
        m_scrollbarsStateTime = m_scrollbarsFadeDuration;
      }

      // update material
      m_renderData->material()->setDiffuseColor(Color(0.25f, 0.25f, 0.25f, 1.0f - m_scrollbarsStateTime.seconds() / m_scrollbarsFadeDuration.seconds()));

      // check if done
      if (m_scrollbarsStateTime == m_scrollbarsFadeDuration)
      {
        m_scrollbarsState = SS_HIDDEN;
      }
      break;

    case SS_FADING_IN:

      // check if done
      if (m_scrollbarsStateTime > m_scrollbarsFadeDuration)
      {
        m_scrollbarsStateTime = m_scrollbarsFadeDuration;
      }

      // update material
      m_renderData->material()->setDiffuseColor(Color(0.25f, 0.25f, 0.25f, m_scrollbarsStateTime.seconds() / m_scrollbarsFadeDuration.seconds()));

      // check if done
      if (m_scrollbarsStateTime == m_scrollbarsFadeDuration)
      {
        m_scrollbarsState = SS_SHOWN;
      }
      break;
  }

  //EGE_PRINT("%d - Time:%.2f", m_scrollbarsState, m_scrollbarsStateTime.seconds());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Enables/disables scrollbars. */
void ScrollableArea::setScrollbarsEnabled(bool set)
{
  m_scrollbarsEnabled = set;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes object. */
void ScrollableArea::removeObject(PObject object)
{
  m_objects.remove(object);
  m_dirtyContent = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes all objects. */
void ScrollableArea::removeAll()
{
  m_objects.clear();
  m_dirtyContent = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates current content with given rectagle. */
void ScrollableArea::updateContent(const Rectf& rectangle)
{
  // update content size
  m_contentSize.x = Math::Max(m_contentSize.x, rectangle.right());
  m_contentSize.y = Math::Max(m_contentSize.y, rectangle.bottom());

  // update scroll range
  m_scrollRange.x = Math::Max(0.0f, m_contentSize.x - m_physics.scale().x);
  m_scrollRange.y = Math::Max(0.0f, m_contentSize.y - m_physics.scale().y);

  // make sure current offset is in range
  m_scrollOffset.x = Math::Min(m_scrollOffset.x, m_scrollRange.x);
  m_scrollOffset.y = Math::Min(m_scrollOffset.y, m_scrollRange.y);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets scrollbars fade duration. */
void ScrollableArea::setScrollbarsFadeDuration(const Time& duration)
{
  m_scrollbarsFadeDuration = duration;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns size of content (in pixels). */
Vector2f ScrollableArea::contentSize()
{
  if (m_dirtyContent)
  {
    // recalculate content size
    recaluclateContentSize();

    // reset flag
    m_dirtyContent = false;
  }

  return m_contentSize;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Recalculates content size. */
void ScrollableArea::recaluclateContentSize()
{
  // reset content size
  m_contentSize = Vector2f::ZERO;

  // go thru all objects
  for (ObjectsList::const_iterator it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    // process according to object id
    switch ((*it)->uid())
    {
      case EGE_OBJECT_UID_OVERLAY_TEXT:
        {
          TextOverlay* overlay = ege_cast<TextOverlay*>((*it).object());

          // update content
          Rectf rectangle(overlay->physics()->position().x, overlay->physics()->position().y, overlay->textSize().x, overlay->textSize().y);
          updateContent(rectangle);
        }
        break;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when internal transformation is changed. */
void ScrollableArea::transformationChanged()
{
  // invalidate content
  m_dirtyContent = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Widget override. Determines size of the dialog (in pixels). */
Vector2i ScrollableArea::size()
{
  return Vector2i(static_cast<s32>(m_physics.scale().x), static_cast<s32>(m_physics.scale().y));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Widget override. Returns TRUE if widget is frameless. */
bool ScrollableArea::isFrameless() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
