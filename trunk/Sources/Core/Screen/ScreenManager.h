#ifndef EGE_CORE_SCREENMANAGER_H
#define EGE_CORE_SCREENMANAGER_H

/** Class managing the screens. Allows stacking up different screens as well as poping top level screens back. Notifies the screens when their view state
    changes.
*/

#include <EGE.h>
#include <EGETime.h>
#include <EGEList.h>
#include <EGEInput.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Renderer;
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

    /* Renders all screens. */
    void render(Viewport* viewport, Renderer* renderer);
    /* Updates manager. */
    void update(const Time& time);
    /* Shows given screen. */
    void showScreen(PScreen screen);
    /* Hides current (top) screen. */
    void hideScreen();
    /* Removes given screen from stack. */
    void removeScreen(PScreen screen);

  private slots:

    /* Pointer event receiver. */
    void pointerEvent(PPointerData data);

  private:

    /*! List of screens. Sorted from the bottom one to top one (currently visible). */
    EGEList<PScreen> m_screens;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SCREENMANAGER_H