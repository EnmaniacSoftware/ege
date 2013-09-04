#ifndef EGE_CORE_UI_PUSHBUTTON_H
#define EGE_CORE_UI_PUSHBUTTON_H

/** Push buttons are specialization of Widgets.
 */

#include "EGEString.h"
#include "EGETime.h"
#include "EGEAlignment.h"
#include "EGEMap.h"
#include "EGEOverlay.h"
#include "EGESignal.h"
#include "EGERenderComponent.h"
#include "Core/UI/Widget.h"
#include "Core/Component/Physics/PhysicsComponent.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IRenderer;
EGE_DECLARE_SMART_CLASS(PushButton, PPushButton)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PushButton : public Widget
{
  public: 

    PushButton(Application* app, const String& name, egeObjectDeleteFunc deleteFunc = NULL);
    virtual ~PushButton();
    
    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public signals:

    /*! Signal emitted when pointer is released over the button. */
    Signal1<const PPushButton&> clicked;

  public:

    /*! Creates instance of widget. This method is a registration method for factory. */
    static PWidget Create(Application* app, const String& name);

  public:

    /*! @see Widget::isValid. */
    bool isValid() const;
    /*! @see Widget::update. */
    void update(const Time& time) override;
    /*! @see Widget::addForRendering. */
    void addForRendering(IRenderer* renderer, const Matrix4f& transform = Matrix4f::IDENTITY) override;
    /*! @see Widget::setSize. */
    void setSize(const Vector2f& size) override;
    /*! Sets alignment. */
    void setAlignment(Alignment align);
    /*! Pointer event processor. */
    void pointerEvent(PPointerData data);
    /*! Returns physics component. */
    PhysicsComponent& physics() { return m_physics; }
    /*! Returns current alignment. */
    const Alignment& alignment() const { return m_alignment; }
    /*! Sets content text. */
    void setText(const Text& text);
    /*! Set font. */
    void setFont(PFont font);

  private:

    /*! @see Widget::generateRenderData. */
    void generateRenderData() override;
    /*! @see Widget::isFrameless. */
    bool isFrameless() const override;
    /*! @see Widget::initialize. */
    bool initialize(const Dictionary& params) override;
    /*! @see Widget::contentSize. */
    Vector2f contentSize();

  private:

    /*! Alignment. */
    Alignment m_alignment;
    /*! Click flag. TRUE if pointer was down over the button. */
    bool m_clicked;
    /*! Render component. */
    PRenderComponent m_renderData;
    /*! Font in use. */
    PFont m_font;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_UI_PUSHBUTTON_H