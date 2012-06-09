#ifndef EGE_CORE_PAGEDSCROLLABLEAREA_H
#define EGE_CORE_PAGEDSCROLLABLEAREA_H

#include <EGETime.h>
#include <EGEGraphics.h>
#include <EGEInput.h>
#include <EGEPhysics.h>
#include <EGEList.h>
#include <EGEMap.h>
#include "Core/UI/UIScrollView.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(UIPageIndicator, PUIPageIndicator)
EGE_DECLARE_SMART_CLASS(UIScrollView, PUIScrollView)
EGE_DECLARE_SMART_CLASS(UIScrollablePageView, PUIScrollablePageView)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class UIScrollablePageView : public UIScrollView
{
  public:

    UIScrollablePageView(Application* app, const String& name, egeObjectDeleteFunc deleteFunc = NULL);
    virtual ~UIScrollablePageView();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /* Creates instance of widget. This method is a registration method for factory. */
    static PWidget Create(Application* app, const String& name);

  public:

    typedef List<PObject> ObjectsList;

  public:

    /* Widget override. Updates object. */
    void update(const Time& time) override;
    /* Widget override. Pointer event processor. */
    void pointerEvent(PPointerData data) override;
    /* Widget override. Renders object. */
    void addForRendering(Renderer* renderer, const Matrix4f& transform = Matrix4f::IDENTITY) override;
    /* Widget override. Constructs object. */
    EGEResult construct() override;

    /* Enables/disables page indicator. */
    void setPageIndicatorEnabled(bool set);
    /* Returns TRUE if page indicator is enabled. */
    bool isPageIndicatorEnabled() const;

    /* Sets number of pages. */
    void setPageCount(s32 count);
    /* Sets current page. Animates from current position if required. */
    void setPage(s32 page, bool animate);
    /* Returns current page. */
    s32 page() const;

    /* Adds object to a given page. */
    EGEResult addObject(PObject object, u32 pageIdx);
    /* Removes object. */
    void removeObject(PObject object);
    /* Returns object with a given name. */
    PObject object(const String& name) const;
    /* Returns list of all objects for a given page. 
       @param pageIdx Page index for which objects should be returned. If negative object from all pages will be returned.
     */
    ObjectsList objects(s32 pageIdx = -1) const;
    /* Removes all objects. */
    void removeAllObjects();

  private slots:

    /* Slot called when size of widget changes. */
    void onSizeChanged(const Vector2f& size);
    /* Slot called when position of widget changes. */
    void onPositionChanged(const Vector4f& position);

  private:

    /* Widget override. Initializes widget from dictionary. */
    bool initialize(const Dictionary& params) override;
    
    /* Recalculates content area. */
    void recalculateContentArea();
    /* Returns page size. */
    Vector2f pageSize() const;
    
    /* UIScrollView override. Ends move. */
    void endMove() override;
  
  private:

    typedef Map<s32, ObjectsList> PageObjects;

  private:

    /*! Page indicator. */
    PUIPageIndicator m_pageIndicator;
    /*! Map of all assigned objects (page, object-list). */
    PageObjects m_objects;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PAGEDSCROLLABLEAREA_H