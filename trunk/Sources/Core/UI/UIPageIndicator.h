#ifndef EGE_CORE_UI_PAGE_INDICATOR_H
#define EGE_CORE_UI_PAGE_INDICATOR_H

/** Class representing page indicator widget.
 */

#include "Core/UI/Widget.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(UIPageIndicator, PUIPageIndicator)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class UIPageIndicator : public Widget
{
  public:

    UIPageIndicator(Application* app, const String& name, egeObjectDeleteFunc deleteFunc = NULL);
    virtual ~UIPageIndicator();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /* Creates instance of widget. This method is a registration method for factory. */
    static PWidget Create(Application* app, const String& name);
    
  public:

    /* Widget override. Renders object. */
    void addForRendering(Renderer* renderer, const Matrix4f& transform = Matrix4f::IDENTITY) override;
    /* Sets page count. */
    void setPageCount(s32 count);
    /* Returns number of pages. */
    s32 pageCount() const;
    /* Sets current page. */
    void setCurrentPage(s32 pageIdx);
    /* Returns current page. */
    s32 page() const;

  private:

    /* Widget override. Returns TRUE if widget is frameless. */
    bool isFrameless() const override;
    /* Widget override. Initializes widget from dictionary. */
    bool initialize(const Dictionary& params) override;
    /* Widget override. Constructs object. */
    EGEResult construct() override;
    /* Updates render data. */
    void updateRenderData();

  private:

    /*! Render component. */
    PRenderComponent m_renderData;
    /*! Render data validity flag. */
    bool m_renderDataInvalid;
    /*! Number of pages. */
    s32 m_pageCount;
    /*! Current page. */
    s32 m_page;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_UI_PAGE_INDICATOR_H