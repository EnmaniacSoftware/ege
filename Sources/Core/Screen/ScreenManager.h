#ifndef EGE_CORE_SCREENMANAGER_H
#define EGE_CORE_SCREENMANAGER_H

/** Class managing the screens. Allows stacking up different screens as well as poping top level screens back. Notifies the screens when their view state
    changes.
*/

#include "EGE.h"
#include "EGETime.h"
#include "EGEList.h"
#include "EGEInput.h"
#include "EGEEvent.h"
#include "Core/Engine/Interface/EngineModule.h"
#include "Core/Screen/Interface/IScreenManager.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IRenderer;
class Viewport;
EGE_DECLARE_SMART_CLASS(Screen, PScreen)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ScreenManager : public EngineModule<IScreenManager>
                    , public IEventListener
{
  public:

    ScreenManager(Engine& engine);
   ~ScreenManager();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  private:

    /*! Returns engine object. */
    Engine& engine() const;

    /*! @see EngineModule::uid. */
    u32 uid() const override;
    /*! @see EngineModule::update. */
    void update(const Time& time) override;

    /*! @see IScreenManager::render. */
    void render(Viewport* viewport, IRenderer* renderer) override;
    /*! @see IScreenManager::show. */
    void show(PScreen screen) override;
    /*! @see IScreenManager::hide. */
    void hide() override;
    /*! @see IScreenManager::hideAll. */
    void hideAll() override;
    /*! @see IScreenManager::remove. */
    void remove(PScreen screen) override;
    /*! @see IScreenManager::removeAll. */
    void removeAll() override;
    /*! @see IScreenManager::screen. */
    PScreen screen(const String& name) const override;
    /*! @see IScreenManager::top. */
    PScreen top() const override;
    /*! @see IScreenManager::isVisible. */
    bool isVisible(const PScreen& screen) const override;
    /*! @see IScreenManager::screenCount. */
    u32 screenCount() const override;

    /*! @see IEventListener::onEventRecieved. */
    void onEventRecieved(PEvent event) override;

    /*! Notifies all screens about pointer event.
     *  @param  event Pointer event.
     */
    void notifyPointerEvent(const PointerEvent& event);

  private:

    typedef List<PScreen> ScreenList;

  private:

    /*! Reference to engine. */
    Engine& m_engine;
    /*! List of screens. Sorted from the bottom one to top one (currently visible). */
    ScreenList m_screens;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SCREENMANAGER_H