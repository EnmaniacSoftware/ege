#include "Core/Screen/Interface/ScreenManager.h"
#include "Core/Screen/Interface/Screen.h"
#include "EGEEngine.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* KScreenManagerDebugName = "EGEScreenManager";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ScreenManager)
EGE_DEFINE_DELETE_OPERATORS(ScreenManager)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ScreenManager::ScreenManager(Engine& engine) 
: m_engine(engine)
{
  if ( ! engine.eventManager()->registerListener(this))
  {
    egeWarning(KScreenManagerDebugName) << "Could not register event listener!";
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ScreenManager::~ScreenManager()
{
  engine().eventManager()->unregisterListener(this);

  // remove all
  removeAll();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
void ScreenManager::hideAll()
{
  // hide all
  while ( ! m_screens.empty())
  {
    hide();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ScreenManager::render(Viewport* viewport, IRenderer* renderer)
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
void ScreenManager::removeAll()
{
  // remove all
  while ( ! m_screens.empty())
  {
    remove(top());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ScreenManager::onEventRecieved(PEvent event)
{
  switch (event->id())
  {
    case EGE_EVENT_ID_CORE_INPUT_EVENT:

      if ((EGE_OBJECT_UID_INPUT_TOUCH_EVENT == event->data()->uid()) ||
          (EGE_OBJECT_UID_INPUT_MOUSE_EVENT == event->data()->uid()))
      {
        notifyPointerEvent(*ege_pcast<PPointerEvent>(event->data()).object());
      }
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ScreenManager::notifyPointerEvent(const PointerEvent& event)
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
        screen->onPointerEvent(event);
        return;
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
PScreen ScreenManager::top() const
{
  return m_screens.empty() ? NULL : m_screens.back();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ScreenManager::isVisible(const PScreen& screen) const
{
//  bool visibility = true;

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
u32 ScreenManager::screenCount() const
{
  return static_cast<u32>(m_screens.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Engine& ScreenManager::engine() const
{
  return m_engine;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 ScreenManager::uid() const
{
  return EGE_OBJECT_UID_SCREEN_MANAGER_MODULE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END