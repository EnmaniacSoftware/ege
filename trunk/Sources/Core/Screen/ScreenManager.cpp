#include "Core/Screen/Screen.h"
#include "Core/Screen/ScreenManager.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(ScreenManager)
EGE_DEFINE_DELETE_OPERATORS(ScreenManager)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ScreenManager::ScreenManager(Application* app) : Object(app)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ScreenManager::~ScreenManager()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates manager. */
void ScreenManager::update(const Time& time)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Shows given screen. */
void ScreenManager::showScreen(PScreen& screen)
{
  // check if some screen is being shown
  if (!m_screens.empty())
  {
    // leave currently shown screen
    m_screens.back()->leave();
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

