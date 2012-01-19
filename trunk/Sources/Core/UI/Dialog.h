#ifndef EGE_CORE_DIALOG_H
#define EGE_CORE_DIALOG_H

#include <EGEString.h>
#include <EGETime.h>
#include <EGEAlignment.h>
#include <EGEMap.h>
#include <EGEOverlay.h>
#include "Core/UI/ScrollableArea.h"
#include "Core/Components/Render/RenderComponent.h"
#include "Core/Components/Physics/PhysicsComponent.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Renderer;
class Viewport;

EGE_DECLARE_SMART_CLASS(Dialog, PDialog)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Dialog : public Object
{
  public: 

    Dialog(Application* app, const String& name, egeObjectDeleteFunc deleteFunc = NULL);
    virtual ~Dialog();
    
    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Updates overlay. */
    void update(const Time& time);
    /* Renders dialog. */
    void addForRendering(Renderer* renderer, const Matrix4f& transform = Matrix4f::IDENTITY);
    /* Sets alignment. */
    void setAlignment(Alignment align);
    /* Pointer event processor. */
    void pointerEvent(PPointerData data);
    /*! Returns name. */
    const String& name() const { return m_name; }
    /*! Returns physics component. */
    PhysicsComponent& physics() { return m_physics; }
    /*! Returns TRUE if overlay is visible. */
    bool visible() const { return m_visible; }
    /* Sets visibility. */
    void setVisible(bool set);
    /*! Returns current alignment. */
    const Alignment& alignment() const { return m_alignment; }
    /* Sets material. */
    void setMaterial(const PMaterial& material);

    /* Sets rectanlges (in pixels). */
    void setRects(const Recti& topLeft, const Recti& topMiddle, const Recti& topRight, const Recti& middleLeft, const Recti& fill, const Recti& middleRight, 
                  const Recti& bottomLeft, const Recti& bottomMiddle, const Recti& bottomRight);

    /* Sets max size. */
    void setMaxSize(const Vector2i& size);
    /* Adds content area. */
    EGEResult addContentArea(const String& name, const Rectf& rect, bool verticalScroll = false, bool horizontalScroll = false);
    /* Adds text overlay to given content area. */
    EGEResult addTextOverlay(const String& contentName, PTextOverlay& overlay);

  private:

    /* Generates render data. */
    void generateRenderData();
    /* Determines size of the dialog (in pixels). */
    Vector2i size();
    /* Determines size of the content only (in pixels). */
    Vector2i contentSize() const;
    /* Determines size of the frame only, without any content (in pixels). */
    Vector2i noContentSize() const;

  private:

    /*! Content area data struct. */
    struct ContentAreaData
    {
      PScrollableArea area;                 /*!< Scrollable area container. */
      Rectf rect;                           /*!< Rectangle for container (relative and normalized). */
    };

    typedef Map<String, ContentAreaData> ContentAreaDataMap;

  private:

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
    /*! Alignment. */
    Alignment m_alignment;
    /*! Cached dialog size (in pixels). */
    Vector2i m_size;
    /*! Size cache validity flag. */
    bool m_sizeValid;

    /*! Top-left rect with (in pixels). This is relative to dialog texture. */
    Recti m_topLeftRect;
    /*! Top-middle rect with (in pixels). This is relative to dialog texture. */
    Recti m_topMiddleRect;
    /*! Top-right rect with (in pixels). This is relative to dialog texture. */
    Recti m_topRightRect;
    /*! Middle-left rect with (in pixels). This is relative to dialog texture. */
    Recti m_middleLeftRect;
    /*! Fill rect with (in pixels). This is relative to dialog texture. */
    Recti m_fillRect;
    /*! Middle-right rect with (in pixels). This is relative to dialog texture. */
    Recti m_middleRightRect;
    /*! Bottom-left rect with (in pixels). This is relative to dialog texture. */
    Recti m_bottomLeftRect;
    /*! Bottom-middle rect with (in pixels). This is relative to dialog texture. */
    Recti m_bottomMiddleRect;
    /*! Bottom-right rect with (in pixels). This is relative to dialog texture. */
    Recti m_bottomRightRect;
    /*! Maximal dialog size. If zero, there is no max size restriction. */
    Vector2i m_maxSize;
    /*! Map of content areas. */
    ContentAreaDataMap m_contentAreas;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DIALOG_H