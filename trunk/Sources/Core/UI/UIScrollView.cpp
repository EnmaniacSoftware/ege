#include "Core/UI/UIScrollView.h"
#include "Core/UI/UIHorizontalScrollbar.h"
#include "Core/UI/UIVerticalScrollbar.h"
#include <EGEOverlay.h>
#include <EGEMath.h>
#include <EGETweener.h>
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define DEFAULT_SCROLLBAR_SIZE 5
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(UIScrollView)
EGE_DEFINE_DELETE_OPERATORS(UIScrollView)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UIScrollView::UIScrollView(Application* app, const String& name, u32 uid, egeObjectDeleteFunc deleteFunc) : Widget(app, name, uid, deleteFunc),
                                                                                                            m_state(STATE_IDLE),
                                                                                                            m_contentSize(Vector2f::ZERO),
                                                                                                            m_maxOffset(Vector2f::ZERO),
                                                                                                            m_scrollDirections(DIRECTION_BOTH),
                                                                                                            m_offset(Vector2f::ZERO),
                                                                                                            m_decelerationRate(0.95f),
                                                                                                            m_animationDuration(0.75f),
                                                                                                            m_throwCoefficient(10.0f),
                                                                                                            m_scrollbarsNeedUpdate(true)
{
  ege_connect(this, sizeChanged, this, UIScrollView::onSizeChanged);
  ege_connect(this, positionChanged, this, UIScrollView::onPositionChanged);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UIScrollView::~UIScrollView()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult UIScrollView::construct()
{
  EGEResult result;

  // call base class
  if (EGE_SUCCESS != (result = Widget::construct()))
  {
    // error!
    return result;
  }

  // allocate scrollbars
  m_verticalScroll = UIVerticalScrollbar::Create(app(), name() + "-vertical-scrollbar");
  if (NULL == m_verticalScroll)
  {
    // error!
    return EGE_ERROR;
  }

  m_horizontalScroll = UIHorizontalScrollbar::Create(app(), name() + "-horizontal-scrollbar");
  if (NULL == m_horizontalScroll)
  {
    // error!
    return EGE_ERROR;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool UIScrollView::isFrameless() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool UIScrollView::initialize(const Dictionary& params)
{
  // initialize base
  bool error = !Widget::initialize(params);

  return !error;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIScrollView::setContentSize(const Vector2f& size)
{
  if (m_contentSize != size)
  {
    // set new content size
    m_contentSize = size;

    // update max content offset
    m_maxOffset.x = Math::Max(0.0f, m_contentSize.x - this->size().x);
    m_maxOffset.y = Math::Max(0.0f, m_contentSize.y - this->size().y);

    // make sure current offset is in range
    m_offset.x = Math::Min(m_offset.x, m_maxOffset.x);
    m_offset.y = Math::Min(m_offset.y, m_maxOffset.y);

    // invalidate scrollbars
    m_scrollbarsNeedUpdate = true;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIScrollView::update(const Time& time)
{
  // check if animating
  if (STATE_ANIMATING == m_state)
  {
    m_animationTime += time;
    if (m_animationTime >= m_animationDuration)
    {
      m_animationTime = m_animationDuration;
      
      // reset state
      m_state = STATE_IDLE;

      // reset data
      m_lastPointerPosition = m_currentPointerPosition;
      m_scrollVelocity      = Vector2f::ZERO;
    }

    // interpolate
    PowerTweener tweener;
    Math::Lerp(&m_offset, &m_animationStartOffset, &m_animationEndOffset, tweener.easeOut(m_animationTime.seconds() / m_animationDuration.seconds()));
  }
  else
  {
    const float32 cutOffValue = 0.1f;

    // upate scrolling
    Vector2f displacement = m_lastPointerPosition - m_currentPointerPosition;

    // limit movement according to direction
    if (0 == (m_scrollDirections & DIRECTION_VERTICAL))
    {
      displacement.y = 0.0f;
    }

    if (0 == (m_scrollDirections & DIRECTION_HORIZONTAL))
    {
      displacement.x = 0.0f;
    }

    // check if dragging
    if (STATE_DRAGGING == m_state)
    {
      // while content is being dragged scroll immediately to final position
      m_offset += displacement;

      // calculate momentum based on durrent displacement
      m_scrollVelocity = displacement * m_throwCoefficient;
    }
    else
    {
      // check if outside boundary
      if (0 > m_offset.x)
      {
        m_scrollVelocity.x = -m_offset.x * m_decelerationRate;
      }
      else if (cutOffValue < (m_offset.x - m_maxOffset.x))
      {
        m_scrollVelocity.x = -(m_offset.x - m_maxOffset.x) * m_decelerationRate;
      }

      if (0 > m_offset.y)
      {
        m_scrollVelocity.y = -m_offset.y * m_decelerationRate;
      }
      else if (cutOffValue < (m_offset.y - m_maxOffset.y))
      {
        m_scrollVelocity.y = -(m_offset.y - m_maxOffset.y) * m_decelerationRate;
      }

      // update position
      m_offset += m_scrollVelocity * time.seconds();

      // dampen velocity
      m_scrollVelocity.x *= m_decelerationRate;
      m_scrollVelocity.y *= m_decelerationRate;

      if (cutOffValue > Math::Abs(m_scrollVelocity.x))
      {
        m_scrollVelocity.x = 0.0f;
      }

      if (cutOffValue > Math::Abs(m_scrollVelocity.y))
      {
        m_scrollVelocity.y = 0.0f;
      }

      // check if scrollbars should be hidden
      if (Vector2f::ZERO == m_scrollVelocity)
      {
        // hide scrollbars
        m_verticalScroll->startHiding();
        m_horizontalScroll->startHiding();
      }
    }
  
    //EGE_PRINT("Off: %.2f %.2f %d", m_offset.x, m_offset.y, m_state);

    // update cache
    m_lastPointerPosition = m_currentPointerPosition;

    // process scrollbars if necessary
    //if (m_scrollbarsEnabled)
    //{
    //  // check if scrollbars should be hidden
    //  if ((Vector2f::ZERO == m_scrollVelocity) && (SS_SHOWN == m_scrollbarsState) && (STATE_DRAGGED != m_state))
    //  {
    //    // hide scrollbars
    //    hidesScrollbars();
    //  }

    //  // check if update is required
    //  if (SS_HIDDEN != m_scrollbarsState)
    //  {
    //    // update scrollbars
    //    updateScrollbars(time);
    //  }
    //}
  }

  // update scrollbars
  m_verticalScroll->setOffset(static_cast<s32>(m_offset.y));
  m_horizontalScroll->setOffset(static_cast<s32>(m_offset.x));

  m_verticalScroll->update(time);
  m_horizontalScroll->update(time);

  // call base class
  Widget::update(time);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIScrollView::pointerEvent(PPointerData data)
{
  // check if inside widget
  const Matrix4f& globalMatrix = globalTransformationMatrix();
  Rectf rect(globalMatrix.translation().x, globalMatrix.translation().y, size().x, size().y);

  bool inside = rect.contains(static_cast<float32>(data->x()), static_cast<float32>(data->y()));
  if (!inside)
  {
    // terminate any drag ongoing
    if (STATE_DRAGGING == m_state)
    {
      // end move
      endMove();
    }
  }
  else
  {
    // process according to action
    if (ACTION_BUTTON_DOWN == data->action())
    {
      // begin move
      beginMove(data->x(), data->y());
    }
    else if (ACTION_BUTTON_UP == data->action() && (STATE_DRAGGING == m_state))
    {
      // end move
      endMove();
    }
    else if ((ACTION_MOVE == data->action()) && (STATE_DRAGGING == m_state))
    {
      // store current pointer position
      m_currentPointerPosition.x = static_cast<float32>(data->x());
      m_currentPointerPosition.y = static_cast<float32>(data->y());
    }
  }

  // call base class
  Widget::pointerEvent(data);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIScrollView::beginMove(s32 x, s32 y)
{
  // store current pointer position
  m_currentPointerPosition.x = static_cast<float32>(x);
  m_currentPointerPosition.y = static_cast<float32>(y);

  // initially last and current are the same
  m_lastPointerPosition = m_currentPointerPosition;

  // change state
  m_state = STATE_DRAGGING;

  // show scrollbars
  m_verticalScroll->startShowing();
  m_horizontalScroll->startShowing();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIScrollView::endMove()
{
  // change state
  m_state = STATE_IDLE;

  Vector2f scrollToOffset = m_offset;

  scrollToOffset.x = Math::Bound(m_offset.x, 0.0f, m_maxOffset.x);
  scrollToOffset.y = Math::Bound(m_offset.y, 0.0f, m_maxOffset.y);

  setOffset(scrollToOffset, true);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const Vector2f& UIScrollView::offset() const
{
  return m_offset;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIScrollView::setOffset(const Vector2f& offset, bool animate)
{
  // check if dragging
  if (STATE_DRAGGING == m_state)
  {
    // do nothing
    return;
  }

  if (offset != m_offset)
  {
    // check if view should be animated
    if (animate)
    {
      // setup animation
      m_animationStartOffset  = m_offset;
      m_animationEndOffset.x  = Math::Min(offset.x, m_maxOffset.x);
      m_animationEndOffset.y  = Math::Min(offset.y, m_maxOffset.y);
      m_animationTime         = 0.0f;

      // set state
      m_state = STATE_ANIMATING;
    }
    else
    {
      // go to offset immedialely
      m_offset = offset;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIScrollView::setDirection(Direction direction)
{
  m_scrollDirections = direction;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIScrollView::setDecelerationRate(float32 rate)
{
  m_decelerationRate = rate;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const Vector2f& UIScrollView::scrollVelocity() const
{
  return m_scrollVelocity;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 UIScrollView::throwCoefficient() const
{
  return m_throwCoefficient;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIScrollView::addForRendering(Renderer* renderer, const Matrix4f& transform)
{
  if (!isVisible())
  {
    // do nothing
    return;
  }

  // check if scrollbars needs update
  if (m_scrollbarsNeedUpdate)
  {
    updateScrollbars();
    m_scrollbarsNeedUpdate = false;
  }

  // render scrollbars
  m_verticalScroll->addForRendering(renderer, transform);
  m_horizontalScroll->addForRendering(renderer, transform);

  // call base class
  Widget::addForRendering(renderer, transform);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIScrollView::onSizeChanged(const Vector2f& size)
{
  EGE_UNUSED(size);

  // invalidate scrollbars
  m_scrollbarsNeedUpdate = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIScrollView::onPositionChanged(const Vector4f& position)
{
  EGE_UNUSED(position);

  // invalidate scrollbars
  m_scrollbarsNeedUpdate = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIScrollView::updateScrollbars()
{
  m_verticalScroll->setPosition(Vector4f(m_physics.position().x + size().x - DEFAULT_SCROLLBAR_SIZE, m_physics.position().y, 0));
  m_verticalScroll->setSize(Vector2f(DEFAULT_SCROLLBAR_SIZE, size().y));
  m_verticalScroll->setRange(0, static_cast<s32>(m_contentSize.y));
  m_verticalScroll->setPageSize(static_cast<s32>(size().y));
  
  m_horizontalScroll->setPosition(Vector4f(m_physics.position().x, m_physics.position().y + size().y - DEFAULT_SCROLLBAR_SIZE, 0));
  m_horizontalScroll->setSize(Vector2f(size().x, DEFAULT_SCROLLBAR_SIZE));
  m_horizontalScroll->setRange(0, static_cast<s32>(m_contentSize.x));
  m_horizontalScroll->setPageSize(static_cast<s32>(size().x));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIScrollView::setScrollbarsEnabled(bool set)
{
  m_verticalScroll->setVisible(set);
  m_horizontalScroll->setVisible(set);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void UIScrollView::setBaseRenderPriority(s32 priority)
{
  m_verticalScroll->renderData()->setPriority(priority + 1);
  m_horizontalScroll->renderData()->setPriority(priority + 1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END