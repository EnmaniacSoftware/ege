#include "Core/Application/Application.h"
#include "Core/Screen/Screen.h"
#include "Core/Screen/ScreenManager.h"
#include <EGESignal.h>

EGE_NAMESPACE

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

    // go thru all screens from top to bottom and update first enabled one
    // NOTE: no const_reverse_iterator due to GCC incompatibility of operator== and operator!=
    for (ScreenList::reverse_iterator it = localCopy.rbegin(); it != localCopy.rend(); ++it)
    {
      PScreen& screen = *it;      
      if (screen->isEnabled())
      {
        screen->update(time);
        return;
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Shows given screen adding it on top of the rest ones. */
void ScreenManager::show(PScreen screen)
{
  // check if some screen is being shown
  if (!m_screens.empty())
  {
    // if current screen has no transparency, previous one can leave immediatly
    if (!screen->hasTransparency())
    {
      m_screens.back()->leave();
    }
    else
    {
      // just cover it
      m_screens.back()->cover();
    }
  }

  // add to pool
  m_screens.push_back(screen);

  // enter new screen
  screen->enter();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Hides current (top) screen. */
void ScreenManager::hide()
{
  bool wasTransparent = false;

  // check if some screen is being shown
  if (!m_screens.empty())
  {
    // leave currently shown screen
    m_screens.back()->leave();

    // remove it
    m_screens.pop_back();
  }

  // check if any screen present
  if (!m_screens.empty())
  {
    // enter top screen
    m_screens.back()->enter();
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
/*! Removes given screen from stack. */
void ScreenManager::remove(PScreen screen)
{
  m_screens.remove(screen);
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
