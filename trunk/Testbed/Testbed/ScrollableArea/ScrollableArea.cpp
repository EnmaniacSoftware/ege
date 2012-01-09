#include "ScrollableArea.h"
#include "App.h"
#include <EGEOverlay.h>
#include <EGEMath.h>
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ScrollableArea)
EGE_DEFINE_DELETE_OPERATORS(ScrollableArea)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ScrollableArea::ScrollableArea(Application* app) : m_app(app),
                                                   m_state(STATE_IDLE),
                                                   m_dampingCoefficient(0.95f, 0.95f),
                                                   m_throwCoefficient(25.0f, 25.0f),
                                                   m_baseReturnCoefficient(5.0f, 5.0f),
                                                   m_direction(DIRECTION_BOTH)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ScrollableArea::~ScrollableArea()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates object. */
void ScrollableArea::update(const Time& time)
{
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
  Vector2f displacement = m_currentPointerPosition - m_lastPointerPosition;

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
    m_scrollOffset -= displacement;

    // no velocity as we go to final position immediately
    m_scrollVelocity = Vector2f::ZERO;
  }
  else
  {
    // check if outside boundary
    if (0 > m_scrollOffset.x)
    {
      m_scrollVelocity.x = -m_scrollOffset.x * m_baseReturnCoefficient.x;
    }
    else if (m_scrollOffset.x > m_scrollRange.x)
    {
      m_scrollVelocity.x = -(m_scrollOffset.x - m_scrollRange.x) * m_baseReturnCoefficient.x;
    }
    else
    {
      m_scrollVelocity.x -= displacement.x * m_throwCoefficient.x;
    }

    if (0 > m_scrollOffset.y)
    {
      m_scrollVelocity.y = -m_scrollOffset.y * m_baseReturnCoefficient.y;
    }
    else if (m_scrollOffset.y > m_scrollRange.y)
    {
      m_scrollVelocity.y = -(m_scrollOffset.y - m_scrollRange.y) * m_baseReturnCoefficient.y;
    }
    else
    {
      m_scrollVelocity.y -= displacement.y * m_throwCoefficient.y;
    }
  }

  // update cache
  m_lastPointerPosition = m_currentPointerPosition;

  // update position
  m_scrollOffset += m_scrollVelocity * time.seconds();

  m_scrollVelocity.x *= m_dampingCoefficient.x;
  m_scrollVelocity.y *= m_dampingCoefficient.y;
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
void ScrollableArea::render(Renderer* renderer)
{
  Matrix4f matrix = Matrix4f::IDENTITY;
  Vector4f pos = m_physics.position();
  pos.x -= m_scrollOffset.x;
  pos.y -= m_scrollOffset.y;

  Math::CreateMatrix(&matrix, &pos, &Vector4f::ONE, &Quaternionf::IDENTITY);

  for (ObjectsList::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    switch ((*it)->uid())
    {
      case EGE_OBJECT_UID_OVERLAY_TEXT:
        {
          TextOverlay* overlay = ege_cast<TextOverlay*>(*it);

          overlay->renderData()->setClipRect(Rectf(m_physics.position().x, m_physics.position().y, m_physics.scale().x, m_physics.scale().y));
          renderer->addForRendering(overlay->renderData(), matrix.multiply(overlay->physics()->transformationMatrix()));
        }
        break;
    }
  }
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
      {
        TextOverlay* overlay = ege_cast<TextOverlay*>(object);

        // update content size
        m_contentSize.x = Math::Max(m_contentSize.x, overlay->physics()->position().x + overlay->textSize().x);
        m_contentSize.y = Math::Max(m_contentSize.y, overlay->physics()->position().y + overlay->textSize().y);

        // update scroll range
        m_scrollRange.x = Math::Max(0.0f, m_contentSize.x - m_physics.scale().x);
        m_scrollRange.y = Math::Max(0.0f, m_contentSize.y - m_physics.scale().y);

        // add to pool
        m_objects.push_back(object);

      }
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
