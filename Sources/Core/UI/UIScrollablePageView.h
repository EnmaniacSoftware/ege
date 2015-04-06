#ifndef EGE_CORE_UI_SCROLLABLEPAGEVIEW_H
#define EGE_CORE_UI_SCROLLABLEPAGEVIEW_H

/** Specialization of UIScrollView. Allows adding content to a set of pages and controls the scrolling between pages.
 */

#include "EGETime.h"
#include "EGEGraphics.h"
#include "EGEInput.h"
#include "EGEPhysics.h"
#include "EGEList.h"
#include "EGEMap.h"
#include "Core/UI/UIScrollView.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_PREDECLARE_SMART_CLASS(UIPageIndicator, PUIPageIndicator)
EGE_PREDECLARE_SMART_CLASS(UIScrollView, PUIScrollView)
EGE_PREDECLARE_SMART_CLASS(UIScrollablePageView, PUIScrollablePageView)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class UIScrollablePageView : public UIScrollView
{
  public:

    UIScrollablePageView(Engine& engine, const String& name, egeObjectDeleteFunc deleteFunc = NULL);
    virtual ~UIScrollablePageView();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Creates instance of widget. This method is a registration method for factory. */
    static PWidget Create(Engine& engine, const String& name);

  public:

    typedef List<PObject> ObjectsList;

  public:

    /*! @see UIScrollView::update. */
    void update(const Time& time) override;
    /*! @see UIScrollView::addForRendering. */
    void addForRendering(IRenderer* renderer, const Matrix4f& transform = Matrix4f::IDENTITY) override;
    /*! @see UIScrollView::construct. */
    EGEResult construct() override;

    /*! Enables/disables page indicator. */
    void setPageIndicatorEnabled(bool set);
    /*! Returns TRUE if page indicator is enabled. */
    bool isPageIndicatorEnabled() const;

    /*! Sets number of pages. */
    void setPageCount(s32 count);
    /*! Sets current page. Animates from current position if required. */
    void setPage(s32 page, bool animate);
    /*! Returns current page. */
    s32 page() const;
    /*! Returns page indicator. */
    PUIPageIndicator pageIndicator() const;

    /*! Adds object to a given page. */
    EGEResult addObject(PObject object, u32 pageIdx);
    /*! Removes object. */
    void removeObject(PObject object);
    /*! Returns object with a given name. */
    PObject object(const String& name) const;
    /*! Returns list of all objects for a given page. 
     *  @param pageIdx Page index for which objects should be returned. If negative object from all pages will be returned.
     */
    ObjectsList objects(s32 pageIdx = -1) const;
    /*! Removes all objects. */
    void removeAllObjects();

    /*! @see UIScrollView::setBaseRenderPriority. */
    void setBaseRenderPriority(s32 priority) override;

  private slots:

    /*! @see UIScrollView::onSizeChanged. */
    void onSizeChanged(const Vector2f& size) override;
    /*! @see UIScrollView::onPositionChanged. */
    void onPositionChanged(const Vector4f& position) override;

  private:

    /*! @see Widget::initialize. */
    bool initialize(const Dictionary& params) override;

    /*! Recalculates content area. */
    void recalculateContentArea();
    /*! Returns page size. */
    Vector2f pageSize() const;
    
    /*! @see UIScrollView::endMove. */
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

#endif // EGE_CORE_UI_SCROLLABLEPAGEVIEW_H