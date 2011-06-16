#ifndef EGE_CORE_SCREENMANAGER_H
#define EGE_CORE_SCREENMANAGER_H

#include <EGE.h>
#include <EGETime.h>
#include <EGEList.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(Screen, PScreen)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ScreenManager : public Object
{
  public:

    ScreenManager(Application* app);
   ~ScreenManager();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Updates manager. */
    void update(const Time& time);
    /* Shows given screen. */
    void showScreen(PScreen& screen);
    /* Hides current (top) screen. */
    void hideScreen();

  private:

    /*! List of screens. Sorted from the bottom one to top one (currently visible). */
    EGEList<PScreen> m_screens;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SCREENMANAGER_H