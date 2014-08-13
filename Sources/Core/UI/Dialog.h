#ifndef EGE_CORE_UI_DIALOG_H
#define EGE_CORE_UI_DIALOG_H

/** Dialogs are specialization of Widgets. Dialogs typically have title and main content areas.
 */

#include "EGEString.h"
#include "EGETime.h"
#include "EGEAlignment.h"
#include "EGEMap.h"
#include "EGERenderComponent.h"
#include "Core/UI/Widget.h"
#include "Core/Component/Physics/PhysicsComponent.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IRenderer;
EGE_DECLARE_SMART_CLASS(Dialog, PDialog)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Dialog : public Widget
{
  public: 

    Dialog(Engine& engine, const String& name, egeObjectDeleteFunc deleteFunc = NULL);
    virtual ~Dialog();
    
    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Creates instance of widget. This method is a registration method for factory. */
    static PWidget Create(Engine& engine, const String& name);

  public:

    /*! @see Widget::isValid. */
    bool isValid() const;
    /*! @see Widget::update. */
    void update(const Time& time) override;
    /*! @see Widget::addForRendering. */
    void addForRendering(IRenderer* renderer, const Matrix4f& transform = Matrix4f::IDENTITY) override;
    /*! @see Widget::setAlpha. */
    void setAlpha(float32 alpha) override;
    /*! Pointer event processor. */
    void pointerEvent(PPointerData data);
    /*! Returns physics component. */
    PhysicsComponent& physics() { return m_physics; }
    /*! Sets title text. */
    void setTitle(const Text& title);
    /*! Sets content text. */
    void setText(const Text& text);
    /*! Sets title font. */
    void setTitleFont(PFont font);
    /*! Sets text font. */
    void setTextFont(PFont font);

  private:

    /*! @see Widget::generateRenderData. */
    void generateRenderData() override;
    /*! @see Widget::isFrameless. */
    bool isFrameless() const override;
    /*! @see Widget::initialize. */
    bool initialize(const Dictionary& params) override;

  private:

    /*! Tail render component. */
    PRenderComponent m_tailRenderData;
    /*! Title font. */
    PFont m_titleFont;
    /*! Text font. */
    PFont m_textFont;
    /*! Tail rectangle part (in pixels). This also defines texture coordinates. */
    Recti m_tailRect;
    /*! Tail offset (in pixels). */
    Vector2f m_tailOffset;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_UI_DIALOG_H