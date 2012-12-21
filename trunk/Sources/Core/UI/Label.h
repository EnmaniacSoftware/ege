#ifndef EGE_CORE_UI_LABEL_H
#define EGE_CORE_UI_LABEL_H

/** Labels are specialization of Widgets.
 */

#include <EGEString.h>
#include <EGETime.h>
#include <EGEAlignment.h>
#include <EGEMap.h>
#include <EGESignal.h>
#include <EGEOverlay.h>
#include "Core/UI/Widget.h"
#include "Core/Components/Render/RenderComponent.h"
#include "Core/Components/Physics/PhysicsComponent.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Renderer;
EGE_DECLARE_SMART_CLASS(Label, PLabel)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Label : public Widget
{
  public: 

    Label(Application* app, const String& name, egeObjectDeleteFunc deleteFunc = NULL);
    virtual ~Label();
    
    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Creates instance of widget. This method is a registration method for factory. */
    static PWidget Create(Application* app, const String& name);

  public:

    /*! @see Widget::isValid. */
    bool isValid() const;
    /*! @see Widget::update. */
    void update(const Time& time) override;
    /*! @see Widget::addForRendering. */
    void addForRendering(Renderer* renderer, const Matrix4f& transform = Matrix4f::IDENTITY) override;
    /*! @see Widget::setAlpha. */
    void setAlpha(float32 alpha) override;
    /*! Pointer event processor. */
    void pointerEvent(PPointerData data);
    /*! Returns physics component. */
    PhysicsComponent& physics() { return m_physics; }
    /*! Sets content text. */
    void setText(const Text& text);
    /*! Sets font. */
    void setFont(PFont font);

    /*! Sets text alignment. */
    void setTextAlignment(Alignment alignment);

  private:

    /*! @see Widget::isFrameless. */
    bool isFrameless() const override;
    /*! @see Widget::initialize. */
    bool initialize(const Dictionary& params) override;
    /*! @see Widget::contentSize. */
    Vector2f contentSize();

  private:

    /*! Text overlay. */
    PTextOverlay m_textOverlay;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_UI_LABEL_H