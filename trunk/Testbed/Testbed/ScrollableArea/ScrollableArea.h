#ifndef SCROLLABLE_AREA_H
#define SCROLLABLE_AREA_H

#include <EGETime.h>
#include <EGEGraphics.h>
#include <EGEInput.h>
#include <EGEPhysics.h>
#include <EGEList.h>
#include <EGEFlags.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Game;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ScrollableArea
{
  public:

    ScrollableArea(EGE::Application* app);
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
    void update(const EGE::Time& time);
    /* Pointer event processor. */
    void pointerEvent(EGE::PPointerData data);
    /* Renders object. */
    void render(EGE::Renderer* renderer);
    /*! Returns local physics. */
    inline EGE::PhysicsComponent& physics() { return m_physics; }
    /* Adds object. */
    EGE::EGEResult addObject(EGE::PObject object);
    /* Removes object. */
    void removeObject(EGE::PObject object);
    /* Removes all objects. */
    void removeAll();
    /* Sets scroll direction. */
    void setDirection(Direction direction);
    /* Sets scroll damping coefficient. */
    void setDampingCoefficient(const EGE::Vector2f& coefficient);
    /* Sets scroll throw coefficient. */
    void setThrowCoefficient(const EGE::Vector2f& coefficient);
    /* Sets scroll base return coefficient. */
    void setBaseReturnCoefficient(const EGE::Vector2f& coefficient);
    /* Enables/disables scrollbars. */
    void setScrollbarsEnabled(bool set);

  private:

    /* Shows scrollbars. */
    void showScrollbars();
    /* Hides scrollbars. */
    void hidesScrollbars();
    /* Updates scrollbars. */
    void updateScrollbars(const EGE::Time& time);
    /* Updates current content with given size. */
    void updateContent(const EGE::Rectf& rectangle);

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

    typedef EGE::List<EGE::PObject> ObjectsList;

  private:

    /*! Pointer to application object. */
    EGE::Application* m_app;
    /*! Local physics. */
    EGE::PhysicsComponent m_physics;
    /*! Content size (in pixels). */
    EGE::Vector2f m_contentSize;
    /*! List of attached objects. */
    ObjectsList m_objects;
    /*! Current state. */
    State m_state;
    /*! Current scroll velocity. */
    EGE::Vector2f m_scrollVelocity;
    /*! Current scroll offset. */
    EGE::Vector2f m_scrollOffset;
    /*! Current scroll range. */
    EGE::Vector2f m_scrollRange;
    /*! Last pointer position (absolute). */
    EGE::Vector2f m_lastPointerPosition;
    /*! Current pointer position (absolute). */
    EGE::Vector2f m_currentPointerPosition;
    /*! Damping coefficient. */
    EGE::Vector2f m_dampingCoefficient;
    /*! Throw coefficient. 
     *  @note This is how fast the scrolling initially is when pointer is released.
     */
    EGE::Vector2f m_throwCoefficient;
    /*! Base return coefficient. *
     *  @note This is how fast the scrolling proceedes when pointer was released outside the available range.
     */
    EGE::Vector2f m_baseReturnCoefficient;
    /*! Current scroll direction. */
    Direction m_direction;
    /*! Scollbar visibility flag. */
    bool m_scrollbarsEnabled;
    /*! Scrollbars current state time. */
    EGE::Time m_scrollbarsStateTime;
    /*! Scrollbars fade duration. */
    EGE::Time m_scrollbarsFadeDuration;
    /*! Current scrollbars state. */
    ScrollbarState m_scrollbarsState;
    /*! Scrollbar render component. */
    EGE::PRenderComponent m_scrollbarRenderData;
    /*! Dirty content flag. */
    bool m_dirtyContent;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SCROLLABLE_AREA_H