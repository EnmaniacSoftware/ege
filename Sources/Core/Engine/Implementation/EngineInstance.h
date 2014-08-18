#ifndef EGE_CORE_ENGINE_ENGINEOBJECT_H
#define EGE_CORE_ENGINE_ENGINEOBJECT_H

/*! Engine implementation.
 */

#include "EGEEngine.h"
#include "EGETime.h"
#include "EGEDictionary.h"
#include "EGEEvent.h"
#include "Core/Engine/Interface/EngineInternal.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class EngineApplication;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class EngineInstance : public Engine
                     , public EngineInternal
                     , public IEventListener
{
  public:

    explicit EngineInstance(const Dictionary& commandLineDictionary);
   ~EngineInstance();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Returns current state. */
    EngineState state() const;

  private:

    /*! @see Engine::commandLineDictionary. */
    const Dictionary& commandLineDictionary() const override;
    /*! @see Engine::con  truct. */
    EGEResult construct() override;
    /*! @see Engine::graphics. */
    Graphics* graphics() const override;
    /*! @see Engine::eventManager. */
    EventManager* eventManager() const override;
    /*! @see Engine::physicsManager. */
    PhysicsManager* physicsManager() const override;
    /*! @see Engine::sceneManager. */
    SceneManager* sceneManager() const override;
    /*! @see Engine::resourceManager. */
    ResourceManager* resourceManager() const override;
    /*! @see Engine::pointer. */
    Pointer* pointer() const override;
    /*! @see Engine::overlayManager. */
    OverlayManager* overlayManager() const override;
    /*! @see Engine::screenManager. */
    ScreenManager* screenManager() const override;
    /*! @see Engine::audioManager. */
    IAudioManager* audioManager() const override;
    /*! @see Engine::debug. */
    Debug* debug() const override;
    /*! @see Engine::deviceServices. */
    DeviceServices* deviceServices() const override;
    /*! @see Engine::purchaseServices. */
    PurchaseServices* purchaseServices() const override;
    /*! @see Engine::socialServices. */
    SocialServices* socialServices() const override;
    /*! @see Engine::imageLoader. */
    ImageLoader* imageLoader() const override;
    /*! Sets new language. */
    void setLanguage(const String& language) override;
    /*! Returns current language. */
    const String& language() const override;
  
    /*! @see EngineInternal::update. */
    void update() override;
    /*! @see EngineInternal::render. */
    void render() override;
    /*! @see EngineInternal::adNetworkRegistry. */
    AdNetworkRegistry* adNetworkRegistry() const override;
    /*! @see EngineInternal::shutdown. */
    void shutdown() override;
    /*! @see EngineInternal::isShuttingDown.*/
    bool isShuttingDown() const override;
    /*! @see EngineInternal::isShutDown.*/
    bool isShutDown() const override;

    /*! @see IEventListener::onEventRecieved. */
    void onEventRecieved(PEvent pEvent) override;

  private:

    /*! State. */
    EngineState m_state;
    /*! Commandline dictionary. */
    Dictionary m_commandLineDictionary;
    /*! Application. */
    EngineApplication* m_application;
    /*! Scene manager. */
    SceneManager* m_sceneManager;
    /*! Physics manager. */
    PhysicsManager* m_physicsManager;
    /*! Event manager. */
    EventManager* m_eventManager;
    /*! Graphics subsystem obejct. */
    Graphics* m_graphics;
    /*! Resource manager. */
    ResourceManager* m_resourceManager;
    /*! Pointer input. */
    Pointer* m_pointer;
    /*! Overlay manager. */
    OverlayManager* m_overlayManager;
    /*! Screen manager. */
    ScreenManager* m_screenManager;
    /*! Audio manager. */
    IAudioManager* m_audioManager;
    /*! Image loader. */
    ImageLoader* m_imageLoader;
    /*! Debug object. */
    Debug* m_debug;
    /*! Device services object. */
    DeviceServices* m_deviceServices;
    /*! Purchase services object. */
    PurchaseServices* m_purchaseServices;
    /*! Social services object. */
    SocialServices* m_socialServices;
    /*! Ad network registry. */
    AdNetworkRegistry* m_adNetworkRegistry;
    /*! Time at which last update was done. */
    Time m_lastUpdateTime;
    /*! Time interval between updates. */
    Time m_updateInterval;
    /*! Time interval between renders. */
    Time m_renderInterval;
    /*! Currently accumulated update duration. */
    Time m_updateAccumulator;
    /*! Current language identifier. */
    String m_language;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_ENGINE_ENGINEOBJECT_H