#ifndef EGE_CORE_APPLICATION_H
#define EGE_CORE_APPLICATION_H

#include "EGE.h"
#include "EGETime.h"
#include "EGESignal.h"
#include "EGEDictionary.h"
#include "EGEVersion.h"
#include "Core/Event/EventListener.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ScreenManager;
class Graphics;
class EventManager;
class ResourceManager;
class PhysicsManager;
class Pointer;
class SceneManager;
class OverlayManager;
class Debug;
class DeviceServices;
class ImageLoader;
class IAudioManager;
class SocialServices;
class PurchaseServices;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Application : public IEventListener
{
  public:

    Application();
    virtual ~Application();

  public signals:

    /*! Signal sent at the begining of a frame. */
    Signal0<> frameBegin;
    /*! Signal sent at the end of a frame. */
    Signal0<> frameEnd;

  public:

    /*! Returns instance of application. This is to be provided by application consuming EGE library. 
     *  @param  Command line parameters
     */
    static Application* CreateInstance();
    /*! Destroys application instance. This is to be provided by application consuming EGE library.
     *  @param  instance Instance to destroy.
     *  @note Instance being destroyed should be previously created by @ref Application::CreateInstance.
     */
    static void DestroyInstance(Application* instance);

  public:

    /*! Available states. */
    enum State
    {
      STATE_INVALID = 0,
      STATE_RUNNING,
      STATE_PAUSED,
      STATE_QUITTING,
      STATE_QUIT
    };

  public:

    /*! Initializes engine.
     *  @param params    List of parameters to initialize engine with.
     *  @param listener  Listener object which is to be notified with engine events. 
     */
    virtual EGEResult construct(const Dictionary& params);
    /*! Starts engine work. */
    virtual EGEResult run();
    /*! Called when application has been initialized. */
    virtual void onInitialized() = 0;
    /*! Returns application version. */
    virtual Version version() const = 0;

    /*! Returns current state. */
    State state() const;

    /*! Requests quit. */
    void quit();
    /*! Returns TRUE if application is quitting. */
    bool isQuitting() const;

    /*! Returns graphics subsystem object. */
    Graphics* graphics() const;
    /*! Returns event manager. */
    EventManager* eventManager() const;
    /*! Returns physics manager. */
    PhysicsManager* physicsManager() const;
    /*! Returns scene manager. */
    SceneManager* sceneManager() const;
    /*! Returns resource manager. */
    ResourceManager* resourceManager() const;
    /*! Returns pointer input. */
    Pointer* pointer() const;
    /*! Returns overlay manager. */
    OverlayManager* overlayManager() const;
    /*! Returns screen manager. */
    ScreenManager* screenManager() const;
    /*! Returns audio manager. */
    IAudioManager* audioManager() const;
    /*! Returns debug object. */
    Debug* debug() const;
    /*! Returns device services object. */
    DeviceServices* deviceServices() const;
    /*! Returns purchase services object. */
    PurchaseServices* purchaseServices() const;
    /*! Returns social services object. */
    SocialServices* socialServices() const;
    /*! Returns image loader object. */
    ImageLoader* imageLoader() const;

    /*! Returns TRUE if landscape mode is enabled. */
    bool isLandscape() const;
    /*! Returns current FPS indication. */
    s32 fps() const;
    /*! Sets new language. */
    void setLanguage(const String& language);
    /*! Returns current language. */
    const String& language() const;

    /*! Returns last frame update duration. */
    const Time& lastFrameUpdateDuration() const;

  protected:

    /*! Application updater. */
    virtual void update(const Time& time);
    /*! Application renderer. */
    virtual void render();
    /*! @see IEventListener::onEventRecieved. */
    void onEventRecieved(PEvent pEvent) override;

  private:

    /*! Application internal updater. */
    void update();
    /*! Loads extra configuration from the file. */
    void loadConfig();

  private slots:

    /*! Slot called when resource group has been loaded. */
    void onGroupLoadComplete(const String& name);
    /*! Slot called when resource group could not be loaded. */
    void onGroupLoadError(const String& name);

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(Application)

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
    /*! Landscape mode flag. */
    bool m_landscapeMode;
    /*! Current language identifier. */
    String m_language;
    /*! Last frame update duration. */
    Time m_lastFrameUpdateDuration;
    /*! Time at which last update was done. */
    Time m_lastUpdateTime;
    /*! Time interval between updates. */
    Time m_updateInterval;
    /*! Time interval between renders. */
    Time m_renderInterval;
    /*! Currently accumulated update duration. */
    Time m_updateAccumulator;
    /*! Current state. */
    State m_state;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline Graphics* Application::graphics() const 
{ 
  return m_graphics; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline EventManager* Application::eventManager() const 
{ 
  return m_eventManager; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline PhysicsManager* Application::physicsManager() const 
{ 
  return m_physicsManager; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline SceneManager* Application::sceneManager() const 
{ 
  return m_sceneManager; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline ResourceManager* Application::resourceManager() const 
{ 
  return m_resourceManager; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline Pointer* Application::pointer() const 
{ 
  return m_pointer; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline OverlayManager* Application::overlayManager() const 
{ 
  return m_overlayManager; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline ScreenManager* Application::screenManager() const 
{ 
  return m_screenManager; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline IAudioManager* Application::audioManager() const 
{ 
  return m_audioManager; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline Debug* Application::debug() const 
{ 
  return m_debug; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline DeviceServices* Application::deviceServices() const 
{ 
  return m_deviceServices; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline PurchaseServices* Application::purchaseServices() const
{
  return m_purchaseServices;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline SocialServices* Application::socialServices() const
{
  return m_socialServices;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline ImageLoader* Application::imageLoader() const 
{ 
  return m_imageLoader; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline bool Application::isLandscape() const
{
  return m_landscapeMode; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline const String& Application::language() const 
{ 
  return m_language; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline Application::State Application::state() const 
{ 
  return m_state; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline const Time& Application::lastFrameUpdateDuration() const
{
  return m_lastFrameUpdateDuration;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_APPLICATION_H