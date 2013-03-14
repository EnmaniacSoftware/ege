#ifndef EGE_CORE_SCREENMANAGER_H
#define EGE_CORE_SCREENMANAGER_H

/** Class managing the screens. Allows stacking up different screens as well as poping top level screens back. Notifies the screens when their view state
    changes.
*/

#include "EGE.h"
#include "EGETime.h"
#include "EGEList.h"
#include "EGEInput.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IRenderer;
class Viewport;
EGE_DECLARE_SMART_CLASS(Screen, PScreen)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ScreenManager : public Object
{
  public:

    ScreenManager(Application* app);
   ~ScreenManager();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Creates object. */
    EGEResult construct();
    /*! Renders all screens. */
    void render(Viewport* viewport, IRenderer* renderer);
    /*! Updates manager. */
    void update(const Time& time);
    /*! Shows given screen adding it on top of the rest ones. 
     *  @note New screen should be added (always) (to ensure other can query visibility correctly)
     *        current top screen should be COVERED (always)
     *        if new and top screen are BOTH transparent do nothing (visibility does not change)
     *        if new and top screens are BOTH opaque do nothing (visibility does not change)
     *        if new screen is transparent and top screen is opaque do nothing (visibility does not change)
     *        if new screen is opaque and top screen is transparent then cover all transparent ones going from top to bottom
     *        ENTER new
     */
    void show(PScreen screen);
    /*! Hides current (top) screen. 
     *  @note Current top screen should be LEAVED (always)
     *        Top screen should be removed (always)
     *        Below screen should always be ENTERED
     *        If popped and top screens are BOTH transparent do nothing (visibility does not change)
     *        If popped and top screens are BOTH opaque do nothing (visibility does not change)
     *        If popped screen is transparent and top screen is opaque do nothing (visibility does not change)
     *        If popped screen is opaque and top screen is transparent then cover all transparent ones going from top to bottom
     */
    void hide();
    /*! Removes given screen from stack. 
     *  @note This does not call leave on screen being removed.
     */
    void remove(PScreen screen);
    /*! Returns first screen with given name. */
    PScreen screen(const String& name) const;
    /*! Returns top screen. */
    PScreen top() const;
    /*! Returns TRUE if given screen is visible. */
    bool isVisible(const PScreen& screen) const;

  private slots:

    /*! Pointer event receiver. */
    void pointerEvent(PPointerData data);

  private:

    typedef List<PScreen> ScreenList;

  private:

    /*! List of screens. Sorted from the bottom one to top one (currently visible). */
    ScreenList m_screens;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SCREENMANAGER_H