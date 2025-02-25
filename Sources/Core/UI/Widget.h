#ifndef EGE_CORE_UI_WIDGET_H
#define EGE_CORE_UI_WIDGET_H

/** Widget class represents a base for all kind of widgets.
 */

#include "EGEString.h"
#include "EGETime.h"
#include "EGEInput.h"
#include "EGEMap.h"
#include "EGESignal.h"
#include "EGERenderComponent.h"
#include "Core/UI/WidgetFrame.h"
#include "Core/Component/Physics/PhysicsComponent.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IRenderer;
class Viewport;
EGE_DECLARE_SMART_CLASS(Widget, PWidget)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Widget : public Object
{
  public: 

    virtual ~Widget();
    
    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public signals:

    /*! Signal emitted when size of the widget changes. */
    Signal1<const Vector2f&> sizeChanged;
    /*! Signal emitted when position of the widget changes. */
    Signal1<const Vector4f&> positionChanged;

  public:

    /*! Returns TRUE if object is valid. */
    virtual bool isValid() const;
    /*! Initializes widget from dictionary. */
    virtual bool initialize(const Dictionary& params);
    /*! Updates overlay. */
    virtual void update(const Time& time);
    /*! Renders widget. */
    virtual void addForRendering(IRenderer* renderer, const Matrix4f& transform = Matrix4f::IDENTITY);
    /*! Pointer event processor. */
    virtual void pointerEvent(PPointerData data);
    /*! Returns name. */
    const String& name() const { return m_name; }
    /*! Sets name. */
    void setName(const String& name);
    /*! Returns physics component. */
    PhysicsComponent& physics() { return m_physics; }
    /*! Sets visibility. */
    void setVisible(bool set);
    /*! Returns visibility flag. */
    bool isVisible() const { return m_visible; }
    /*! Sets material. */
    void setMaterial(const PMaterial& material);
    /*! Returns widget frame. */
    WidgetFrame* widgetFrame() const { return m_widgetFrame; }
    
    /*! Adds child. */
    EGEResult addChild(PWidget widget);
    /*! Removes given child. */
    void removeChild(PWidget widget);
    /*! Removes child with a given name. */
    void removeChild(const String& name);
    /*! Removes all children. */
    void removeAllChildren();
    /*! Returns child with a given name. */
    PWidget child(const String& name) const;

    /*! Returns pointer to parent widget. NULL if no parent is set. */
    Widget* parent() const { return m_parent; }

    /*! Detrmines widget's content size (in pixels). */
    virtual Vector2f contentSize();
    /*! Sets size. */
    virtual void setSize(const Vector2f& size);
    /*! Determines size of widget (in pixels). */
    virtual Vector2f size() const;
    /*! Sets position. */
    void setPosition(const Vector4f& position);

    /*! Sets transparency level. */
    virtual void setAlpha(float32 alpha);

    /*! Sets widget alignment. */
    void setAlignment(Alignment alignment);
    /*! Returns current widget alignment. */
    Alignment alignment() const { return m_alignment; }

  protected:

    Widget(Application* app, const String& name, u32 uid, egeObjectDeleteFunc deleteFunc = NULL);
    /*! Generates render data. */
    virtual void generateRenderData();
    /*! Returns global transformation matrix. */
    const Matrix4f& globalTransformationMatrix() const;
    /*! Constructs object. */
    virtual EGEResult construct();

  private:

    /*! Returns TRUE if widget is frameless. */
    virtual bool isFrameless() const = 0;

  private slots:

    /*! Slot called when own transformation has been changed. */
    void onTransformationChanged();

  protected:

    /*! Child data. */
    struct ChildData
    {
      PWidget widget;                       /*!< Child widget. */
    };

    typedef Map<String, ChildData> ChildrenDataMap;

  protected:

    /*! Name. */
    String m_name;
    /*! Physics component. */
    PhysicsComponent m_physics;
    /*! Cached global transformation matrix. */
    mutable Matrix4f m_globalTransformation;
    /*! Flag indicating whether global transformation matrix is valid. */
    mutable bool m_globalTransformationMatrixInvalid;
    /*! Render data invalid flag. */
    bool m_renderDataInvalid;
    /*! Visibility flag. */
    bool m_visible;
    /*! Widget size (in pixels). */
    mutable Vector2f m_size;
    /*! Widget frame. */
    WidgetFrame* m_widgetFrame;
    /*! Pointer to parent widget. */
    Widget* m_parent;
    /*! Children data map. */
    ChildrenDataMap m_children;
    /*! Widget alignment. */
    Alignment m_alignment;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_UI_WIDGET_H