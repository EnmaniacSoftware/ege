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
    m_screens.back()->update(time);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Shows given screen. */
void ScreenManager::showScreen(PScreen screen)
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

  // enter new screen
  screen->enter();

  // add to pool
  m_screens.push_back(screen);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Hides current (top) screen. */
void ScreenManager::hideScreen()
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
  for (EGEList<PScreen>::reverse_iterator it = m_screens.rbegin(); it != m_screens.rend(); ++it)
  {
    Screen* screen = *it;
    if (!screen->hasTransparency())
    {
      // go back from current screen to the begining of iteration and render all screens on the way
      do
      {
        // render current screen
        screen->render(viewport, renderer);

        // check if we have reached top level screen
        if (it == m_screens.rbegin())
        {
          // done
          return;
        }

        // go to screen above
        --it;

      } while (true);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes given screen from stack. */
void ScreenManager::removeScreen(PScreen screen)
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
      m_screens.back()->pointerEvent(data);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
