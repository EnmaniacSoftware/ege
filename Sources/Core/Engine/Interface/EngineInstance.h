#ifndef EGE_CORE_ENGINE_ENGINEOBJECT_H
#define EGE_CORE_ENGINE_ENGINEOBJECT_H

/*! Engine implementation.
 */

#include "EGEEngine.h"
#include "EGETime.h"
#include "EGEDictionary.h"
#include "EGEEvent.h"
#include "Core/Audio/Interface/AudioManagerFactory.h"
#include "Core/Engine/Interface/EngineInternal.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class EngineApplication;
class IEngineModule;
class IGraphics;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class EngineInstance : public Engine
                     , public EngineInternal
                     , public IEventListener
{
  public:

    EngineInstance();
    virtual ~EngineInstance();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Returns current state. */
    EngineState state() const;

  protected:

    /*! @see Engine::configurationDictionary. */
    const Dictionary& configurationDictionary() const override;
    /*! @see Engine::contruct. */
    EGEResult construct(const Dictionary& configParamDictionary) override;
    /*! @see Engine::graphics. */
    IGraphics* graphics() const override;
    /*! @see Engine::eventManager. */
    IEventManager* eventManager() const override;
    /*! @see Engine::physicsManager. */
    IPhysicsManager* physicsManager() const override;
    /*! @see Engine::sceneManager. */
    ISceneManager* sceneManager() const override;
    /*! @see Engine::resourceManager. */
    IResourceManager* resourceManager() const override;
    /*! @see Engine::pointer. */
    Pointer* pointer() const override;
    /*! @see Engine::overlayManager. */
    IOverlayManager* overlayManager() const override;
    /*! @see Engine::screenManager. */
    IScreenManager* screenManager() const override;
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
    /*! @see EngineInternal::adNetwork. */
    AdNetwork* adNetwork() const override;
    /*! @see EngineInternal::audioManagerFactory. */
    AudioManagerFactory* audioManagerFactory() const override;
    /*! @see EngineInternal::shutdown. */
    void shutdown() override;
    /*! @see EngineInternal::isShuttingDown.*/
    bool isShuttingDown() const override;
    /*! @see EngineInternal::isShutDown.*/
    bool isShutDown() const override;

    /*! @see IEventListener::onEventRecieved. */
    void onEventRecieved(PEvent pEvent) override;

  protected:

    /*! Social services object. */
    SocialServices* m_socialServices;

  private:

    /*! Loads extra configuration from the file. */
    void loadConfig();
    /*! Creates all modules. */
    void createModules();
    /*! Constructs all modules. */
    EGEResult constructModules();

  private:

    /*! Engine modules pool, ordered by priority. */
    typedef Map<u32, IEngineModule*> EngineModulesPool;
    /*! Mapping from engine module ID to engine module itself. */
    typedef Map<u32, IEngineModule*> EngineModuleUIDToObject;

  private:

    /*! State. */
    EngineState m_state;
    /*! Configuration dictionary. */
    Dictionary m_configurationDictionary;
    /*! Application. */
    EngineApplication* m_application;
    /*! Pointer input. */
    Pointer* m_pointer;                   //// encapsulate in Input class aggregate ?
    /*! Image loader. */
    ImageLoader* m_imageLoader;
    /*! Debug object. */
    Debug* m_debug;
    /*! Device services object. */
    DeviceServices* m_deviceServices;
    /*! Purchase services object. */
    PurchaseServices* m_purchaseServices;
    /*! Ad network registry. */
    AdNetworkRegistry* m_adNetworkRegistry;
    /*! Audio manager factory. */
    AudioManagerFactory* m_audioManagerFactory;
    /*! Available ad network solution. */
    AdNetwork* m_adNetwork;
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
    /*! Pool of all available engine modules. 
     *  @note Objects stored here should not be deallocated directly! TAGE - ideally would like to have this pool only.
     */
    EngineModulesPool m_modules;
    /*! Mapping of engine module unique ids to engine modules themselves. 
     *  @note This is used for quick lookups.
     */
    EngineModuleUIDToObject m_modulesUIDToObjects;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_ENGINE_ENGINEOBJECT_H