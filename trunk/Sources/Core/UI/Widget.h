#ifndef EGE_CORE_WIDGET_H
#define EGE_CORE_WIDGET_H

/** Widget class represents a base for all kind of widgets.
 */

#include <EGEString.h>
#include <EGETime.h>
#include <EGEInput.h>
#include <EGEMap.h>
#include "Core/UI/WidgetFrame.h"
#include "Core/Components/Render/RenderComponent.h"
#include "Core/Components/Physics/PhysicsComponent.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Renderer;
class Viewport;

EGE_DECLARE_SMART_CLASS(Widget, PWidget)
EGE_DECLARE_SMART_CLASS(ScrollableArea, PScrollableArea)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Widget : public Object
{
  public: 

    virtual ~Widget();
    
    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if object is valid. */
    virtual bool isValid() const;
    /* Initializes widget from dictionary. */
    virtual bool initialize(const Dictionary& params);
    /* Updates overlay. */
    virtual void update(const Time& time);
    /* Renders dialog. */
    virtual void addForRendering(Renderer* renderer, const Matrix4f& transform = Matrix4f::IDENTITY);
    /* Pointer event processor. */
    virtual void pointerEvent(PPointerData data);
    /*! Returns name. */
    const String& name() const { return m_name; }
    /* Sets name. */
    void setName(const String& name);
    /*! Returns physics component. */
    inline PhysicsComponent& physics() { return m_physics; }
    /*! Returns TRUE if overlay is visible. */
    bool visible() const { return m_visible; }
    /* Sets visibility. */
    void setVisible(bool set);
    /* Sets material. */
    void setMaterial(const PMaterial& material);
    /* Sets max size. */
    void setMaxSize(const Vector2i& size);
    /*! Returns widget frame. */
    inline WidgetFrame* widgetFrame() const { return m_widgetFrame; }
    
    /* Adds child. */
    EGEResult addChild(PWidget widget, const Rectf& relativeRect = Rectf(0, 0, 1, 1));
    /* Removes given child. */
    void removeChild(PWidget widget);
    /* Removes child with a given name. */
    void removeChild(const String& name);
    /* Removes all children. */
    void removeAllChildren();
    /* Returns child with a given name. Optionally, stores child rectangle within parent. */
    PWidget child(const String& name, Rectf* rect = NULL) const;

    /*! Returns pointer to parent widget. NULL if no parent is set. */
    inline Widget* parent() const { return m_parent; }

    /*! Detrmines widget's content size (in pixels). */
    virtual Vector2f contentSize();
    /* Sets size. */
    void setSize(const Vector2f& size);
    /* Sets position. */
    void setPosition(const Vector4f& position);

  protected:

    Widget(Application* app, const String& name, u32 uid, egeObjectDeleteFunc deleteFunc = NULL);
    /* Generates render data. */
    virtual void generateRenderData();
    /* Determines size of widget (in pixels). */
    virtual Vector2f size();

  private:

    /*! Returns TRUE if widget is frameless. */
    virtual bool isFrameless() const = 0;

  protected:

    /*! Child data. */
    struct ChildData
    {
      PWidget widget;                       /*!< Child widget. */
      Rectf rect;                           /*!< Rectangle widget occupies within parent (relative and normalized). */
    };

    typedef Map<String, ChildData> ChildrenDataMap;

  protected:

    /*! Name. */
    String m_name;
    /*! Physics component. */
    PhysicsComponent m_physics;
    /*! Render data invalid flag. */
    bool m_renderDataInvalid;
    /*! Visibility flag. */
    bool m_visible;
    /*! Cached widget size (in pixels). */
    Vector2f m_size;
    /*! Size cache validity flag. */
    bool m_sizeValid;
    /*! Widget frame. */
    WidgetFrame* m_widgetFrame;
    /*! Maximal dialog size. If zero, there is no max size restriction. */
    Vector2i m_maxSize;
    /*! Pointer to parent widget. */
    Widget* m_parent;
    /*! Children data map. */
    ChildrenDataMap m_children;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_WIDGET_H