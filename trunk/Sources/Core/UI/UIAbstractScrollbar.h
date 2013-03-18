#ifndef EGE_CORE_UI_ABSTRACT_SCROLLBAR_H
#define EGE_CORE_UI_ABSTRACT_SCROLLBAR_H

/** Class representing abstract scrollbar widget.
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
EGE_DECLARE_SMART_CLASS(UIAbstractScrollbar, PUIAbstractScrollbar)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class UIAbstractScrollbar : public Widget
{
  public:

    virtual ~UIAbstractScrollbar();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS
    
  public slots:

    /*! Sets current scroll offset to given value. */
    void setOffset(s32 offset);
    /*! Starts show sequence. */
    void startShowing();
    /*! Starts hide sequence. */
    void startHiding();

  public:

    /*! @see Widget::update. */
    void update(const Time& time) override;
    /*! @see Widget::addForRendering. */
    void addForRendering(IRenderer* renderer, const Matrix4f& transform = Matrix4f::IDENTITY) override;

    /*! Sets range. */
    void setRange(s32 from, s32 to);
    /*! Sets page size. */
    void setPageSize(s32 size);
    /*! Returns render data component. */
    const PRenderComponent& renderData() const { return m_renderData; }

  protected:

    UIAbstractScrollbar(Application* app, const String& name, u32 uid, egeObjectDeleteFunc deleteFunc = NULL);
    /*! Widget override. Constructs object. */
    EGEResult construct() override;

  private:

    /*! @see Widget::isFrameless. */
    bool isFrameless() const override;
    /*! @see Widget::initialize. */
    bool initialize(const Dictionary& params) override;
    /*! Updates render data. */
    virtual void updateRenderData() = 0;

  private:

    /*! Available states. */
    enum State
    {
      STATE_HIDDEN,
      STATE_FADING_IN,
      STATE_SHOWN,
      STATE_FADING_OUT
    };

  protected:

    /*! Range start value. */
    s32 m_rangeStart;
    /*! Range end value. */
    s32 m_rangeEnd;
    /*! Page size. */
    s32 m_pageSize;
    /*! Current offset. */
    s32 m_offset;
    /*! Current state. */
    State m_state;
    /*! Current state time. */
    Time m_stateTime;
    /*! Fade duration. */
    Time m_fadeDuration;
    /*! Render component. */
    PRenderComponent m_renderData;
    /*! Render data validity flag. */
    bool m_renderDataInvalid;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_UI_ABSTRACT_SCROLLBAR_H