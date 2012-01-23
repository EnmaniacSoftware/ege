#ifndef EGE_CORE_SCROLLABLEAREA_H
#define EGE_CORE_SCROLLABLEAREA_H

#include <EGETime.h>
#include <EGEGraphics.h>
#include <EGEInput.h>
#include <EGEPhysics.h>
#include <EGEList.h>
#include <EGEFlags.h>
#include "Core/UI/Widget.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(ScrollableArea, PScrollableArea)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ScrollableArea : public Widget
{
  public:

    ScrollableArea(Application* app, const String& name, egeObjectDeleteFunc deleteFunc = NULL);
    virtual ~ScrollableArea();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /* Creates instance of widget. This method is a registration method for factory. */
    static PWidget Create(Application* app, const String& name);

  public:

    /*! Available directions. */
    enum DirectionFlags
    {
      DIRECTION_NONE        = 0x00,
      DIRECTION_VERTICAL    = 0x01,
      DIRECTION_HORIZONTAL  = 0x02,

      DIRECTION_BOTH = DIRECTION_VERTICAL | DIRECTION_HORIZONTAL
    };

    EGE_DECLARE_FLAGS(Direction, DirectionFlags)

    typedef List<PObject> ObjectsList;

  public:

    /* Widget override. Updates object. */
    void update(const Time& time) override;
    /* Widget override. Pointer event processor. */
    void pointerEvent(PPointerData data) override;
    /* Widget override. Renders object. */
    void addForRendering(Renderer* renderer, const Matrix4f& transform = Matrix4f::IDENTITY) override;
    /* Adds object. */
    EGE::EGEResult addObject(PObject object);
    /* Removes object. */
    void removeObject(PObject object);
    /* Returns object with a given name. */
    PObject object(const String& name) const;
    /* Reutrns list of all objects. */
    ObjectsList objects() const;
    /* Removes all objects. */
    void removeAll();
    /* Sets scroll direction. */
    void setDirection(Direction direction);
    /*! Returns scroll direction. */
    inline Direction direction() const { return m_direction; }

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

    /* Widget override. Sets transparency level. */
    void setAlpha(float32 alpha) override;

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
    /* Widget override. Determines size of the widget (in pixels). */
    Vector2f size();
    /* Widget override. Returns TRUE if widget is frameless. */
    bool isFrameless() const override;
    /* Widget override. Initializes widget from dictionary. */
    bool initialize(const Dictionary& params) override;

  private slots:

    /* Slot called when internal transformation is changed. */
    void transformationChanged();

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

  private:

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
    /*! Dirty content flag. */
    bool m_dirtyContent;
    /*! Render component. */
    PRenderComponent m_renderData;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SCROLLABLEAREA_H