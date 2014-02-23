#ifndef EGE_CORE_UI_VERTICAL_SCROLLBAR_H
#define EGE_CORE_UI_VERTICAL_SCROLLBAR_H

/** Class representing vertical scrollbar widget.
 */

#include "Core/UI/UIAbstractScrollbar.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(UIVerticalScrollbar, PUIVerticalScrollbar)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class UIVerticalScrollbar : public UIAbstractScrollbar
{
  public:

    UIVerticalScrollbar(Application* app, const String& name, egeObjectDeleteFunc deleteFunc = NULL);
    virtual ~UIVerticalScrollbar();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Creates instance of widget. This method is a registration method for factory. */
    static PWidget Create(Application* app, const String& name);
    
  private:

    /*! @see UIAbstractScrollbar::updateRenderData. */
    void updateRenderData() override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_UI_VERTICAL_SCROLLBAR_H