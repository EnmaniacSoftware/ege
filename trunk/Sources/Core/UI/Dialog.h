#ifndef EGE_CORE_DIALOG_H
#define EGE_CORE_DIALOG_H

/** Dialogs are specialization of Widgets. Dialogs typically have title and main content areas.
 */

#include <EGEString.h>
#include <EGETime.h>
#include <EGEAlignment.h>
#include <EGEMap.h>
#include "Core/UI/Widget.h"
#include "Core/Components/Render/RenderComponent.h"
#include "Core/Components/Physics/PhysicsComponent.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Renderer;

EGE_DECLARE_SMART_CLASS(Dialog, PDialog)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Dialog : public Widget
{
  public: 

    Dialog(Application* app, const String& name, egeObjectDeleteFunc deleteFunc = NULL);
    virtual ~Dialog();
    
    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

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
    /* Sets title text. */
    void setTitle(const Text& title);
    /* Sets content text. */
    void setText(const Text& text);
    /* Sets title font. */
    void setTitleFont(PFont font);
    /* Sets text font. */
    void setTextFont(PFont font);

  private:

    /* Widget override. Generates render data. */
    void generateRenderData() override;
    /* Widget override. Returns TRUE if widget is frameless. */
    bool isFrameless() const override;
    /* Widget override. Initializes widget from dictionary. */
    bool initialize(const Dictionary& params) override;

  private:

    /*! Alignment. */
    Alignment m_alignment;
    /*! Render component. */
    PRenderComponent m_renderData;
    /*! Title font. */
    PFont m_titleFont;
    /*! Text font. */
    PFont m_textFont;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DIALOG_H