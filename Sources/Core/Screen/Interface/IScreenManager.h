#ifndef EGE_CORE_SCREEN_SCREENMANAGER_INTERFACE_H
#define EGE_CORE_SCREEN_SCREENMANAGER_INTERFACE_H

#include "EGE.h"
#include "EGEString.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IRenderer;
class Viewport;
EGE_DECLARE_SMART_CLASS(Screen, PScreen)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IScreenManager
{
  public:

    virtual ~IScreenManager() {}

  public:

    /*! Renders all screens. TAGE - probably should be removed */
    virtual void render(Viewport* viewport, IRenderer* renderer) = 0;

    /*! Shows given screen adding it on top of the rest ones. 
     *  @note New screen should be added (always) (to ensure other can query visibility correctly)
     *        current top screen should be COVERED (always)
     *        if new and top screen are BOTH transparent do nothing (visibility does not change)
     *        if new and top screens are BOTH opaque do nothing (visibility does not change)
     *        if new screen is transparent and top screen is opaque do nothing (visibility does not change)
     *        if new screen is opaque and top screen is transparent then cover all transparent ones going from top to bottom
     *        ENTER new
     */
    virtual void show(PScreen screen) = 0;
    /*! Hides current (top) screen. 
     *  @note Current top screen should be LEAVED (always)
     *        Top screen should be removed (always)
     *        Below screen should always be ENTERED
     *        If popped and top screens are BOTH transparent do nothing (visibility does not change)
     *        If popped and top screens are BOTH opaque do nothing (visibility does not change)
     *        If popped screen is transparent and top screen is opaque do nothing (visibility does not change)
     *        If popped screen is opaque and top screen is transparent then cover all transparent ones going from top to bottom
     */
    virtual void hide() = 0;
    /*! Hides all screens. */
    virtual void hideAll() = 0;
    /*! Removes given screen from stack. 
     *  @note This does not call leave on screen being removed.
     */
    virtual void remove(PScreen screen) = 0;
    /*! Remoes all screens from stack. 
     *  @note This does not call leave on screens being removed.
     */
    virtual void removeAll() = 0;

    /*! Returns first screen with given name. */
    virtual PScreen screen(const String& name) const = 0;
    /*! Returns top screen. */
    virtual PScreen top() const = 0;
    /*! Returns TRUE if given screen is visible. */
    virtual bool isVisible(const PScreen& screen) const = 0;
    /*! Returns current number of screens on stack. */
    virtual u32 screenCount() const = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SCREEN_SCREENMANAGER_INTERFACE_H