#ifndef EGE_CORE_UI_SCROLLABLE_VIEW_H
#define EGE_CORE_UI_SCROLLABLE_VIEW_H

/** Specialization of UIScrollView. Allows adding content to a 'virtual' canvas and controls the scrolling over it.
 */

#include <EGETime.h>
#include <EGEGraphics.h>
#include <EGEInput.h>
#include <EGEPhysics.h>
#include <EGEList.h>
#include <EGEFlags.h>
#include "Core/UI/UIScrollView.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(UIScrollableView, PUIScrollableView)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class UIScrollableView : public UIScrollView
{
  public:

    UIScrollableView(Application* app, const String& name, egeObjectDeleteFunc deleteFunc = NULL);
    virtual ~UIScrollableView();

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

    /* Adds object. */
    EGEResult addObject(PObject object);
    /* Removes object. */
    void removeObject(PObject object);
    /* Returns object with a given name. */
    PObject object(const String& name) const;
    /* Returns list of all objects. */
    ObjectsList objects() const;
    /* Removes all objects. */
    void removeAllObjects();

    /* UIScrollView override. Sets content size. 
       @note This disables auto content size generation.
     */
    void setContentSize(const Vector2f& size) override;
    /* Enables/disables auto calculation of contect size. */
    void setAutoContentSize(bool enable);

    /* Widget override. Sets transparency level. */
    void setAlpha(float32 alpha) override;

  private:

    /* Widget override. Returns TRUE if widget is frameless. */
    bool isFrameless() const override;
    /* Widget override. Initializes widget from dictionary. */
    bool initialize(const Dictionary& params) override;

    /* Recalculates content size. */
    void recaluclateContentSize();
    /* Returns size of content (in pixels). */
    Vector2f contentSize();

  private slots:

    /* Slot called when internal transformation is changed. */
    void transformationChanged();

  private:

    /*! Content size (in pixels). */
    Vector2f m_contentSize;
    /*! List of attached objects. */
    ObjectsList m_objects;
    /*! Dirty content flag. */
    bool m_dirtyContent;
    /*! Auto content size flag. */
    bool m_autoContentSize;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_UI_SCROLLABLE_VIEW_H