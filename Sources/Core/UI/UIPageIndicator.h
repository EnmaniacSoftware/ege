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

    UIPageIndicator(Engine& engine, const String& name, egeObjectDeleteFunc deleteFunc = NULL);
    virtual ~UIPageIndicator();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Creates instance of widget. This method is a registration method for factory. */
    static PWidget Create(Engine& engine, const String& name);
    
  public:

    /*! @see Widget::addForRendering. */
    void addForRendering(IRenderer* renderer, const Matrix4f& transform = Matrix4f::IDENTITY) override;
    /*! Sets page count. */
    void setPageCount(s32 count);
    /*! Returns number of pages. */
    s32 pageCount() const;
    /*! Sets current page. */
    void setCurrentPage(s32 pageIdx);
    /*! Returns current page. */
    s32 page() const;
    /*! Sets materials for page indicators. */
    void setMaterials(DynamicArray<PMaterial> on, DynamicArray<PMaterial> off);
    /*! Sets render priority. */
    void setRenderPriority(s32 priority);
    /*! Sets single page indicator size. */
    void setIndicatorSize(s32 size);

  private:

    /*! @see Widget::isFrameless. */
    bool isFrameless() const override;
    /*! @see Widget::initialize. */
    bool initialize(const Dictionary& params) override;
    /*! @see Widget::construct. */
    EGEResult construct() override;
    /*! Updates render data. */
    void updateRenderData();

  private:

    /*! Render components for all pages. */
    DynamicArray<PRenderComponent> m_renderData;
    /*! Render data validity flag. */
    bool m_renderDataInvalid;
    /*! Number of pages. */
    s32 m_pageCount;
    /*! Current page. */
    s32 m_page;
    /*! ON indicator materials. */
    DynamicArray<PMaterial> m_onMaterials;
    /*! OFF indicator materials. */
    DynamicArray<PMaterial> m_offMaterials;
    /*! Render priority. */
    s32 m_renderPriority;
    /*! Single page indicator size. */
    s32 m_indicatorSize;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_UI_PAGE_INDICATOR_H