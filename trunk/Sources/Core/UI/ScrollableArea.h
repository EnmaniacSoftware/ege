#ifndef EGE_CORE_SCROLLABLEAREA_H
#define EGE_CORE_SCROLLABLEAREA_H

#include <EGETime.h>
#include <EGEGraphics.h>
#include <EGEInput.h>
#include <EGEPhysics.h>
#include <EGEList.h>
#include <EGEFlags.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(ScrollableArea, PScrollableArea)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ScrollableArea : public Object
{
  public:

    ScrollableArea(Application* app);
   ~ScrollableArea();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Available directions. */
    enum DirectionFlags
    {
      DIRECTION_VERTICAL    = 0x1,
      DIRECTION_HORIZONTAL  = 0x2,

      DIRECTION_BOTH = DIRECTION_VERTICAL | DIRECTION_HORIZONTAL
    };

    EGE_DECLARE_FLAGS(Direction, DirectionFlags)

  public:

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Updates object. */
    void update(const Time& time);
    /* Pointer event processor. */
    void pointerEvent(PPointerData data);
    /* Renders object. */
    void render(Renderer* renderer);
    /*! Returns local physics. */
    inline PhysicsComponent& physics() { return m_physics; }
    /* Adds object. */
    EGE::EGEResult addObject(PObject object);
    /* Removes object. */
    void removeObject(PObject object);
    /* Removes all objects. */
    void removeAll();
    /* Sets scroll direction. */
    void setDirection(Direction direction);
    /* Sets scroll damping coefficient. */
    void setDampingCoefficient(const Vector2f& coefficient);
    /* Sets scroll throw coefficient. */
    void setThrowCoefficient(const Vector2f& coefficient);
    /* Sets scroll base return coefficient. */
    void setBaseReturnCoefficient(const Vector2f& coefficient);
    /* Enables/disables scrollbars. */
    void setScrollbarsEnabled(bool set);
    /* Sets scrollbars fade duration. */
    void setScrollbarsFadeDuration(const Time& duration);
    /* Returns size of content (in pixels). */
    Vector2f contentSize();

  private:

    /* Shows scrollbars. */
    void showScrollbars();
    /* Hides scrollbars. */
    void hidesScrollbars();
    /* Updates scrollbars. */
    void updateScrollbars(const Time& time);
    /* Updates current content with given size. */
    void updateContent(const Rectf& rectangle);
    /* Recalculates content size. */
    void recaluclateContentSize();

  private:

    /*! Available states. */
    enum State
    {
      STATE_IDLE,
      STATE_DRAGGED
    };

    /*! Available scrollbar states. */
    enum ScrollbarState
    {
      SS_HIDDEN,
      SS_FADING_IN,
      SS_SHOWN,
      SS_FADING_OUT
    };

    typedef List<PObject> ObjectsList;

  private:

    /*! Local physics. */
    PhysicsComponent m_physics;
    /*! Content size (in pixels). */
    Vector2f m_contentSize;
    /*! List of attached objects. */
    ObjectsList m_objects;
    /*! Current state. */
    State m_state;
    /*! Current scroll velocity. */
    Vector2f m_scrollVelocity;
    /*! Current scroll offset. */
    Vector2f m_scrollOffset;
    /*! Current scroll range. */
    Vector2f m_scrollRange;
    /*! Last pointer position (absolute). */
    Vector2f m_lastPointerPosition;
    /*! Current pointer position (absolute). */
    Vector2f m_currentPointerPosition;
    /*! Damping coefficient. */
    Vector2f m_dampingCoefficient;
    /*! Throw coefficient. 
     *  @note This is how fast the scrolling initially is when pointer is released.
     */
    Vector2f m_throwCoefficient;
    /*! Base return coefficient. *
     *  @note This is how fast the scrolling proceedes when pointer was released outside the available range.
     */
    Vector2f m_baseReturnCoefficient;
    /*! Current scroll direction. */
    Direction m_direction;
    /*! Scollbar visibility flag. */
    bool m_scrollbarsEnabled;
    /*! Scrollbars current state time. */
    Time m_scrollbarsStateTime;
    /*! Scrollbars fade duration. */
    Time m_scrollbarsFadeDuration;
    /*! Current scrollbars state. */
    ScrollbarState m_scrollbarsState;
    /*! Scrollbar render component. */
    PRenderComponent m_scrollbarRenderData;
    /*! Dirty content flag. */
    bool m_dirtyContent;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SCROLLABLEAREA_H