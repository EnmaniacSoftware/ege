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
    /* Updates overlay. */
    virtual void update(const Time& time);
    /* Renders dialog. */
    virtual void addForRendering(Renderer* renderer, const Matrix4f& transform = Matrix4f::IDENTITY);
    /* Pointer event processor. */
    virtual void pointerEvent(PPointerData data);
    /*! Returns name. */
    const String& name() const { return m_name; }
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
    /* Adds content area. */
    EGEResult addContentArea(const String& name, const Rectf& rect, bool verticalScroll = false, bool horizontalScroll = false);
    
    /* Adds child. */
    EGEResult addChild(PWidget widget);
    /* Removes given child. */
    void removeChild(PWidget widget);
    /* Removes child with a given name. */
    void removeChild(const String& name);
    /* Removes all children. */
    void removeAllChildren();

  protected:

    Widget(Application* app, const String& name, u32 uid, egeObjectDeleteFunc deleteFunc = NULL);
    /* Generates render data. */
    virtual void generateRenderData();
    /* Initializes the object. This method is to be called from derived objects. */
    void initialize();

  private:

    /* Determines size of the dialog (in pixels). */
    virtual Vector2i size();
    /* Determines size of the content only (in pixels). */
    Vector2i contentSize() const;
    /*! Returns TRUE if widget is frameless. */
    virtual bool isFrameless() const = 0;

  protected:

    /*! Content area data struct. */
    struct ContentAreaData
    {
      PScrollableArea area;                 /*!< Scrollable area container. */
      Rectf rect;                           /*!< Rectangle for container (relative and normalized). */
    };

    typedef Map<String, ContentAreaData> ContentAreaDataMap;
    typedef Map<String, PWidget> ChildrenMap;

  protected:

    /*! Name. */
    String m_name;
    /*! Physics component. */
    PhysicsComponent m_physics;
    /*! Render component. */
    PRenderComponent m_renderData;
    /*! Render data invalid flag. */
    bool m_renderDataInvalid;
    /*! Visibility flag. */
    bool m_visible;
    /*! Cached widget size (in pixels). */
    Vector2i m_size;
    /*! Size cache validity flag. */
    bool m_sizeValid;
    /*! Widget frame. */
    WidgetFrame* m_widgetFrame;
    /*! Maximal dialog size. If zero, there is no max size restriction. */
    Vector2i m_maxSize;
    /*! Map of content areas. */
    ContentAreaDataMap m_contentAreas;
    /*! Pointer to parent widget. */
    Widget* m_parent;
    /*! Children widgets. */
    ChildrenMap m_children;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_WIDGET_H