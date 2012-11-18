#include "Core/Application/Application.h"
#include "Core/Screen/Screen.h"
#include "Core/Screen/ScreenManager.h"
#include <EGESignal.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ScreenManager)
EGE_DEFINE_DELETE_OPERATORS(ScreenManager)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ScreenManager::ScreenManager(Application* app) : Object(app)
{
  ege_connect(app->pointer(), eventSignal, this, ScreenManager::pointerEvent);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ScreenManager::~ScreenManager()
{
  ege_disconnect(app()->pointer(), eventSignal, this, ScreenManager::pointerEvent);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates manager. */
void ScreenManager::update(const Time& time)
{
  if (!m_screens.empty())
  {
    ScreenList localCopy;
    localCopy.copy(m_screens);

    // go thru all screens from top to bottom and update all enabled ones untill end is reached or some non-transparent is found
    // NOTE: no const_reverse_iterator due to GCC incompatibility of operator== and operator!=
    for (ScreenList::reverse_iterator it = localCopy.rbegin(); it != localCopy.rend(); ++it)
    {
      PScreen& screen = *it;      
      if (screen->isEnabled())
      {
        // update screen
        if (screen->update(time))
        {
          // done, consumed
          return;
        }

        // check if screen in not transparent
        if (!screen->hasTransparency())
        {
          // no need to update the rest
          return;
        }
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Shows given screen adding it on top of the rest ones. 
 *  @note: new screen should be added (always) (to ensure other can query visibility correctly)
 *         current top screen should be COVERED (always)
 *         if new and top screen are BOTH transparent do nothing (visibility does not change)
 *         if new and top screens are BOTH opaque do nothing (visibility does not change)
 *         if new screen is transparent and top screen is opaque do nothing (visibility does not change)
 *         if new screen is opaque and top screen is transparent then cover all transparent ones going from top to bottom
 *         ENTER new
 */
void ScreenManager::show(PScreen screen)
{
  PScreen topScreen = top();

  // add to pool first
  m_screens.push_back(screen);

  // cover top screen if any
  if (NULL != topScreen)
  {
    topScreen->cover();
  }

  // check if visibility changed
  if ( ! screen->hasTransparency() && (NULL != topScreen) && topScreen->hasTransparency())
  {
    // NOTE: no const_reverse_iterator due to GCC incompatibility of operator== and operator!=
    ScreenList::reverse_iterator it  = m_screens.rbegin();
    ++it;
    ++it;
    ScreenList::reverse_iterator end = m_screens.rend();

    for (; it != end; ++it)
    {
      PScreen& currentScreen = *it;      
      if (currentScreen->hasTransparency())
      {
        currentScreen->cover();
      }
      else
      {
        // we are done
        break;
      }
    }
  }

  // enter new screen
  screen->enter();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Hides current (top) screen. 
 *  @note: Current top screen should be LEAVED (always)
 *         Top screen should be removed (always)
 *         Below screen should always be ENTERED
 *         If popped and top screens are BOTH transparent do nothing (visibility does not change)
 *         If popped and top screens are BOTH opaque do nothing (visibility does not change)
 *         If popped screen is transparent and top screen is opaque do nothing (visibility does not change)
 *         If popped screen is opaque and top screen is transparent then cover all transparent ones going from top to bottom
 */
void ScreenManager::hide()
{
  PScreen topScreen = top();

  // leave top screen if any
  if (NULL != topScreen)
  {
    topScreen->leave();

    // remove from pool
    m_screens.pop_back();
  }

  // enter screen below the one we just removed
  if (NULL != top())
  {
    top()->enter();
  }

  // check if visibility changed and more than 2 screens
  if ( ! topScreen->hasTransparency() && (NULL != top()) && top()->hasTransparency())
  {
    // NOTE: no const_reverse_iterator due to GCC incompatibility of operator== and operator!=
    ScreenList::reverse_iterator it  = m_screens.rbegin();
    ++it;
    ScreenList::reverse_iterator end = m_screens.rend();

    for (; it != end; ++it)
    {
      PScreen& currentScreen = *it;      
      if (currentScreen->hasTransparency())
      {
        currentScreen->cover();
      }
      else
      {
        // we are done
        break;
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Renders all screens. */
void ScreenManager::render(Viewport* viewport, Renderer* renderer)
{
  // go thru all screens from top to bottom
  // NOTE: no const_reverse_iterator due to GCC incompatibility of operator== and operator!=
  for (ScreenList::reverse_iterator it = m_screens.rbegin(); it != m_screens.rend(); ++it)
  {
    // NOTE: do not use references so we dont overwrite entries below while walking up the stack
    Screen* screen = *it;
    if (!screen->hasTransparency() && screen->isEnabled())
    {
      // go back from current screen to the begining of iteration and render all screens on the way
      do
      {
        // render current screen
        if (screen->isEnabled())
        {
          screen->render(viewport, renderer);
        }

        // check if we have reached top level screen
        if (it == m_screens.rbegin())
        {
          // done
          return;
        }

        // go to screen above
        screen = *--it;

      } while (true);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes given screen from stack. 
    @note This does not call leave on screen being removed.
 */
void ScreenManager::remove(PScreen screen)
{
  // check if it is top screen
  if (top() == screen)
  {
    // remove it
    m_screens.remove(screen);

    PScreen currentTop = top();

    // uncover current top if any
    if (NULL != currentTop)
    {
      currentTop->uncover();
    }
  }
  else
  {
    // remove
    m_screens.remove(screen);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Pointer event receiver. */
void ScreenManager::pointerEvent(PPointerData data)
{
  // pass into top-level screen only
  if (!m_screens.empty())
  {
    // go thru all screens from top to bottom and propagate to first enabled one
    // NOTE: no const_reverse_iterator due to GCC incompatibility of operator== and operator!=
    for (ScreenList::reverse_iterator it = m_screens.rbegin(); it != m_screens.rend(); ++it)
    {
      PScreen& screen = *it;      
      if (screen->isEnabled())
      {
        screen->pointerEvent(data);
        return;
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns first screen with given name. */
PScreen ScreenManager::screen(const String& name) const
{
  // go thru all screens
  for (ScreenList::const_iterator it = m_screens.begin(); it != m_screens.end(); ++it)
  {
    // check if found
    if ((*it)->name() == name)
    {
      // found
      return *it;
    }
  }

  // not found
  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns top screen. */
PScreen ScreenManager::top() const
{
  return m_screens.empty() ? NULL : m_screens.back();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if given screen is visible. */
bool ScreenManager::isVisible(const PScreen& screen) const
{
  bool visibility = true;

  // go thru all screens from top to bottom
  // NOTE: no const_reverse_iterator due to GCC incompatibility of operator== and operator!=
  for (ScreenList::const_reverse_iterator it = m_screens.rbegin(); it != m_screens.rend(); ++it)
  {
    const PScreen& currentScreen = *it;     

    // check if reached the desired screen
    if (screen == currentScreen)
    {
      // visible
      return true;
    }

    // check if current screen has no transparency
    if (currentScreen->isEnabled() && ! currentScreen->hasTransparency())
    {
      // not visible
      return false;
    }
  }

  // not found, not visible
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END