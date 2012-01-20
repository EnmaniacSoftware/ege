#ifndef EGE_CORE_PUSHBUTTON_H
#define EGE_CORE_PUSHBUTTON_H

/** Push buttons are specialization of Widgets.
 */

#include <EGEString.h>
#include <EGETime.h>
#include <EGEAlignment.h>
#include <EGEMap.h>
#include <EGEOverlay.h>
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

  public:

    /* Creates instance of widget. This method is a registration method for factory. */
    static PWidget Create(Application* app, const String& name);

  public:

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
    /* Adds text overlay to given content area. */
    EGEResult addTextOverlay(const String& contentName, PTextOverlay& overlay);

  private:

    /* Widget override. Generates render data. */
    void generateRenderData() override;
    /* Widget override. Determines size of the dialog (in pixels). */
    Vector2i size() override;
    /* Determines size of the content only (in pixels). */
    Vector2i contentSize() const;
    /* Widget override. Returns TRUE if widget is frameless. */
    bool isFrameless() const override;

  private:

    /*! Alignment. */
    Alignment m_alignment;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PUSHBUTTON_H