#ifndef EGE_CORE_UI_SCROLLVIEW_H
#define EGE_CORE_UI_SCROLLVIEW_H

/** Base class supporting displaying of the content larger than size of a window. It is intended to be subclassed.
 */

#include "EGETime.h"
#include "EGEGraphics.h"
#include "EGEInput.h"
#include "EGEPhysics.h"
#include "EGEList.h"
#include "EGEFlags.h"
#include "Core/UI/Widget.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(UIScrollView, PUIScrollView)
EGE_DECLARE_SMART_CLASS(UIHorizontalScrollbar, PUIHorizontalScrollbar)
EGE_DECLARE_SMART_CLASS(UIVerticalScrollbar, PUIVerticalScrollbar)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class UIScrollView : public Widget
{
  public:

    virtual ~UIScrollView();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:
    
    /*! Sets content size. */
    virtual void setContentSize(const Vector2f& size);
    /*! Returns current offset vector. */
    const Vector2f& offset() const;
    /*! Sets current offset vector. */
    void setOffset(const Vector2f& offset, bool animate);
    /*! Sets scroll direction. */
    void setDirection(Direction direction);
    /*! Sets deceleration rate. */
    void setDecelerationRate(float32 rate);
    /*! Enables/disables scroll bars. */
    void setScrollbarsEnabled(bool set);
    /*! Sets base render priority. */
    virtual void setBaseRenderPriority(s32 priority);

  protected:

    UIScrollView(Engine& engine, const String& name, u32 uid, egeObjectDeleteFunc deleteFunc = NULL);

    /*! @see Widget::update. */
    void update(const Time& time) override;
    /*! @see Widget::onPointerEvent. */
    void onPointerEvent(const PointerEvent& event) override;
    /*! @see Widget::initialize. */
    bool initialize(const Dictionary& params) override;
    /*! @see Widget::construct. */
    EGEResult construct() override;
    /*! @see Widget::isFrameless. */
    bool isFrameless() const override;
    /*! @see Widget::addForRendering. */
    void addForRendering(IRenderer* renderer, const Matrix4f& transform = Matrix4f::IDENTITY) override;

    /*! Begins move. */
    virtual void beginMove(s32 x, s32 y);
    /*! Ends move. */
    virtual void endMove();

    /*! Returns current scroll velocity. */
    const Vector2f& scrollVelocity() const;
    /*! Returns throw coefficient. */
    float32 throwCoefficient() const;

  protected slots:

    /*! Slot called when size of widget changes. */
    virtual void onSizeChanged(const Vector2f& size);
    /*! Slot called when position of widget changes. */
    virtual void onPositionChanged(const Vector4f& position);

  private:

    /*! Updates scrollbars. */
    void updateScrollbars();

  protected:

    /*! Available states. */
    enum State
    {
      STATE_IDLE,             /*!< Idle, ready to interact. */
      STATE_DRAGGING,         /*!< View is being dragged. */
      STATE_ANIMATING         /*!< View is being animated. */
    };

  protected:

    /*! Current state. */
    State m_state;
    /*! Content size (in pixels). */
    Vector2f m_contentSize;

  private:

    /*! Max offset range (in pixels). */
    Vector2f m_maxOffset;
    /*! Allowed scroll directions. */
    Direction m_scrollDirections;
    /*! Vertuical scroll bar. */
    PUIVerticalScrollbar m_verticalScroll;
    /*! Horizontal scroll bar. */
    PUIHorizontalScrollbar m_horizontalScroll;
    /*! Last pointer position (absolute). */
    Vector2f m_lastPointerPosition;
    /*! Current pointer position (absolute). */
    Vector2f m_currentPointerPosition;
    /*! Current content offset. */
    Vector2f m_offset;
    /*! Decelearation rate when pointer is released. */
    float32 m_decelerationRate;
    /*! Time duration it takes to animate offest from one position into other. */
    Time m_animationDuration;
    /*! Current scroll velocity. */
    Vector2f m_scrollVelocity;
    /*! Start content offset while animating. */
    Vector2f m_animationStartOffset;
    /*! Destination content offset while animating. */
    Vector2f m_animationEndOffset;
    /*! Current animation time. */
    Time m_animationTime;
    /*! Throw coefficient. This is how fast the scrolling initially is when pointer is released. */
    float32 m_throwCoefficient;
    /*! Scrollbars need update flag. */
    bool m_scrollbarsNeedUpdate;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_UI_SCROLLVIEW_H