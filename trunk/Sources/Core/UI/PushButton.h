#ifndef EGE_CORE_PUSHBUTTON_H
#define EGE_CORE_PUSHBUTTON_H

/** Push buttons are specialization of Widgets.
 */

#include <EGEString.h>
#include <EGETime.h>
#include <EGEAlignment.h>
#include <EGEMap.h>
#include <EGEOverlay.h>
#include <EGESignal.h>
#include "Core/UI/Widget.h"
#include "Core/Components/Render/RenderComponent.h"
#include "Core/Components/Physics/PhysicsComponent.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Renderer;

EGE_DECLARE_SMART_CLASS(PushButton, PPushButton)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class PushButton : public Widget
{
  public: 

    PushButton(Application* app, const String& name, egeObjectDeleteFunc deleteFunc = NULL);
    virtual ~PushButton();
    
    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  signals:

    /*! Signal emitted when pointer is released over the button. */
    Signal1<const PPushButton&> clicked;

  public:

    /* Creates instance of widget. This method is a registration method for factory. */
    static PWidget Create(Application* app, const String& name);

  public:

    /* Widget override. Returns TRUE if object is valid. */
    bool isValid() const;
    /* Widget override. Updates overlay. */
    void update(const Time& time) override;
    /* Widget override. Renders dialog. */
    void addForRendering(Renderer* renderer, const Matrix4f& transform = Matrix4f::IDENTITY) override;
    /* Sets alignment. */
    void setAlignment(Alignment align);
    /* Pointer event processor. */
    void pointerEvent(PPointerData data);
    /*! Returns physics component. */
    PhysicsComponent& physics() { return m_physics; }
    /*! Returns current alignment. */
    const Alignment& alignment() const { return m_alignment; }
    /* Sets content text. */
    void setText(const Text& text);

  private:

    /* Widget override. Generates render data. */
    void generateRenderData() override;
    /* Widget override. Returns TRUE if widget is frameless. */
    bool isFrameless() const override;
    /* Widget override. Initializes widget from dictionary. */
    bool initialize(const Dictionary& params) override;
    /* Widget override. Detrmines widget's content size (in pixels). */
    Vector2f contentSize();

  private:

    /*! Alignment. */
    Alignment m_alignment;
    /*! Click flag. TRUE if pointer was down over the button. */
    bool m_clicked;
    /*! Render component. */
    PRenderComponent m_renderData;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PUSHBUTTON_H