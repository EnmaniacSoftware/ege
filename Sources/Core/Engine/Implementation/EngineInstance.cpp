#include "Core/Engine/Implementation/EngineInstance.h"
#include "Core/Graphics/Viewport.h"
#include "Core/Graphics/Camera.h"
#include "Core/Graphics/Render/RenderWindow.h"
#include "Core/Graphics/Render/RenderTarget.h"
#include "Core/Graphics/Render/RenderSystem.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Physics/PhysicsManager.h"
#include "Core/Overlay/OverlayManager.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/ComplexTypes.h"
#include "Core/Event/Event.h"
#include "Core/Input/Pointer.h"
#include "Core/Screen/ScreenManager.h"
#include "Core/Audio/Interface/AudioManager.h"
#include "Core/Audio/Interface/Null/AudioManagerNull.h"
#include "Core/Graphics/Image/ImageLoader.h"
#include "Core/Services/Interface/AdNetworkRegistry.h"
#include "EGEEvent.h"
#include "EGEApplication.h"
#include "EGEDeviceServices.h"
#include "EGEPurchaseServices.h"
#include "EGESocialServices.h"
#include "EGETimer.h"
#include "EGEDirectory.h"
#include "EGEResources.h"
#include "EGEDebug.h"

#ifdef EGE_PLATFORM_WIN32
  //#include "Win32/Application/ApplicationWin32_p.h"
  #include "Core/Audio/Interface/OpenAL/AudioManagerOpenAL.h"
#elif EGE_PLATFORM_AIRPLAY
  #include "Airplay/Application/ApplicationAirplay_p.h"
  #include "Airplay/Audio/AudioManagerAirplay.h"
#elif EGE_PLATFORM_IOS
  //#include "iOS/Application/ApplicationIOS_p.h"
  #include "iOS/Audio/Interface/OpenAL/AudioManagerOpenALIOS.h"
#endif // EGE_PLATFORM_WIN32

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* KEngineDebugName = "EGEEngine";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(EngineInstance)
EGE_DEFINE_DELETE_OPERATORS(EngineInstance)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EngineInstance::EngineInstance(const Dictionary& commandLineDictionary)
: m_commandLineDictionary(commandLineDictionary)
, m_state(EStateInvalid)
, m_application(NULL)
, m_sceneManager(NULL)
, m_physicsManager(NULL) 
, m_eventManager(NULL)
, m_graphics(NULL)
, m_resourceManager(NULL) 
, m_pointer(NULL)
, m_overlayManager(NULL) 
, m_screenManager(NULL)
, m_audioManager(NULL)
, m_debug(NULL)
, m_deviceServices(NULL)
, m_purchaseServices(NULL)
, m_socialServices(NULL)
, m_imageLoader(NULL)
, m_adNetworkRegistry(NULL)
, m_lastUpdateTime(0LL)
, m_updateAccumulator(0LL)
, m_language("en")
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EngineInstance::~EngineInstance()
{
  EngineApplication::DestroyInstance(m_application);
  m_application = NULL;

  EGE_DELETE(m_adNetworkRegistry);
  EGE_DELETE(m_imageLoader);
  EGE_DELETE(m_sceneManager);
  EGE_DELETE(m_imageLoader);
  EGE_DELETE(m_screenManager);
  EGE_DELETE(m_overlayManager);
  EGE_DELETE(m_resourceManager);
  EGE_DELETE(m_graphics);
  EGE_DELETE(m_pointer);
  EGE_DELETE(m_physicsManager);
  EGE_DELETE(m_audioManager);
  EGE_DELETE(m_deviceServices);
  EGE_DELETE(m_purchaseServices);
  EGE_DELETE(m_socialServices);
  EGE_DELETE(m_eventManager);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const Dictionary& EngineInstance::commandLineDictionary() const
{
  return m_commandLineDictionary;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult EngineInstance::construct()
{
  EGEResult result;

  // create application instance
  m_application = EngineApplication::CreateInstance(*this);

  // retrieve engine configuration
  Dictionary configuration(commandLineDictionary());
  configuration.merge(m_application->engineConfiguration());

  // decompose param list
  Dictionary::const_iterator iterLandscape  = configuration.find(EGE_ENGINE_PARAM_LANDSCAPE_MODE);
  Dictionary::const_iterator iterUPS        = configuration.find(EGE_ENGINE_PARAM_UPDATES_PER_SECOND);
  Dictionary::const_iterator iterFPS        = configuration.find(EGE_ENGINE_PARAM_RENDERS_PER_SECOND);

  // check if update rate is given
  if (iterUPS != configuration.end())
  {
    m_updateInterval.fromMiliseconds(1000 / iterUPS->second.toInt());
  }

  // check if render rate is given
  if (iterFPS != configuration.end())
  {
    m_renderInterval.fromMiliseconds(1000 / iterFPS->second.toInt());
  }

  // check landscape mode
  //if (iterLandscape != params.end())
  //{
  //  m_landscapeMode = iterLandscape->second.toBool();
  //}
  
  //// load configuration
  //loadConfig();

  // create device services
  m_deviceServices = ege_new PLATFORM_CLASSNAME(DeviceServices)(*this);

  // create purchase services
  m_purchaseServices = ege_new PLATFORM_CLASSNAME(PurchaseServices)(*this);

  // create social services
  m_socialServices = ege_new PLATFORM_CLASSNAME(SocialServices)(*this);

  // create event manager
  m_eventManager = ege_new EventManager();

  if (EGE_SUCCESS != (result = m_eventManager->construct()))
  {
    // error!
    return result;
  }

  // create image loader
  m_imageLoader = ege_new ImageLoader(*this);

  if (EGE_SUCCESS != (result = m_imageLoader->construct()))
  {
    // error!
    return result;
  }

  // create graphics
  // NOTE: must be before ResourceManager
  m_graphics = ege_new Graphics(*this, configuration);

  if (EGE_SUCCESS != (result = m_graphics->construct()))
  {
    // error!
    return result;
  }

  // create resource manager
  m_resourceManager = ege_new ResourceManager(*this);

  if (EGE_SUCCESS != (result = m_resourceManager->construct()))
  {
    // error!
    return result;
  }

  // create physics manager
  m_physicsManager = ege_new PhysicsManager();

  if (EGE_SUCCESS != (result = m_physicsManager->construct(configuration)))
  {
    // error!
    return result;
  }

  // create scene manager
  m_sceneManager = ege_new SceneManager(*this);

  if (EGE_SUCCESS != (result = m_sceneManager->construct()))
  {
    // error!
    return result;
  }

  // create overlay manager
  m_overlayManager = ege_new OverlayManager();

  if (EGE_SUCCESS != (result = m_overlayManager->construct()))
  {
    // error!
    return result;
  }

  // create pointer input
  m_pointer = ege_new Pointer(*this);

  if (EGE_SUCCESS != (result = m_pointer->construct()))
  {
    // error!
    return result;
  }

  // create screen manager
  m_screenManager = ege_new ScreenManager(*this);

  if (EGE_SUCCESS != (result = m_screenManager->construct()))
  {
    // error!
    return result;
  }

  // create audio manager
  // TAGE - make a decision based on some data, to figure out yet
#if EGE_AUDIO_NULL
  m_audioManager = ege_new AudioManagerNull(*this);
#elif EGE_AUDIO_OPENAL
  #if EGE_PLATFORM_IOS
    m_audioManager = ege_new AudioManagerOpenALIOS(*this);
  #else
    m_audioManager = ege_new AudioManagerOpenAL(*this);
  #endif // EGE_PLATFORM_IOS
#else
  m_audioManager = ege_new AudioManagerAirplay(m_application);
#endif // EGE_AUDIO_NULL

  if (EGE_SUCCESS != (result = m_audioManager->construct()))
  {
    // error!
    return result;
  }

  m_adNetworkRegistry = ege_new AdNetworkRegistry(*this);

  // subscribe for event notifications
  if ( ! eventManager()->addListener(this))
  {
    // error!
    return EGE_ERROR;
  }

  // construct application
  return m_application->construct();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void EngineInstance::update()
{ 
  // initialize update timer (to smooth out first update)
  if (0 == m_lastUpdateTime.microseconds())
  {
    m_lastUpdateTime = Timer::GetMicroseconds();

    // set state
    m_state = EStateRunning;
  }

  // get current time
  Time time(Timer::GetMicroseconds());
    
  // calculate time passed
  Time timeInterval = time - m_lastUpdateTime;
  
  // store frame current time
  m_lastUpdateTime = time;
  
  // dont allow to long changes
  if (0.25f < timeInterval.seconds())
  {
    timeInterval = 0.25f;
  }
  
  // always update event manager first so all event are delivered
  eventManager()->update(m_updateInterval);

  // check if quitting
  if (EStateQuitting == state())
  {
    // update only object which needs time to shut down
    graphics()->update();
    resourceManager()->update(m_updateInterval);
    audioManager()->update(m_updateInterval);
    imageLoader()->update(m_updateInterval);

    // check if ready to quit
    if ((ResourceManager::STATE_CLOSED == resourceManager()->state()) &&
        (IAudioManager::StateClosed == audioManager()->state()) &&
        (ImageLoader::STATE_CLOSED == imageLoader()->state()) &&
        (RenderSystem::STATE_CLOSED == graphics()->renderSystem()->state()))
    {
      // change state
      m_state = EStateClosed;
    }
  }
  else if (EStateRunning == state())
  {
    // update accumulator for updates
    m_updateAccumulator += timeInterval;

    // update as much as requested
    while (m_updateAccumulator > m_updateInterval)
    {
      m_updateAccumulator -= m_updateInterval;
      physicsManager()->update(m_updateInterval);
    }

    graphics()->update();
    imageLoader()->update(m_updateInterval);
    resourceManager()->update(timeInterval);

    audioManager()->update(timeInterval);
    screenManager()->update(timeInterval);
    sceneManager()->update(timeInterval);
    overlayManager()->update(timeInterval);

    m_application->update(timeInterval);

    // interpolate physics by remaining value
    // ..
  }

  // store update duration
 // m_lastFrameUpdateDuration = Timer::GetMicroseconds() - m_lastUpdateTime.microseconds();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void EngineInstance::render()
{
  if (EStateRunning == state())
  {
    // do render
    graphics()->render();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void EngineInstance::shutdown()
{
  m_state = EStateQuitting;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool EngineInstance::isShuttingDown() const
{
  return (EStateQuitting == state());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool EngineInstance::isShutDown() const
{
  return (EStateClosed == state());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Graphics* EngineInstance::graphics() const 
{ 
  return m_graphics; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EventManager* EngineInstance::eventManager() const 
{ 
  return m_eventManager; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PhysicsManager* EngineInstance::physicsManager() const 
{ 
  return m_physicsManager; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SceneManager* EngineInstance::sceneManager() const 
{ 
  return m_sceneManager; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceManager* EngineInstance::resourceManager() const 
{ 
  return m_resourceManager; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Pointer* EngineInstance::pointer() const 
{ 
  return m_pointer; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
OverlayManager* EngineInstance::overlayManager() const 
{ 
  return m_overlayManager; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ScreenManager* EngineInstance::screenManager() const 
{ 
  return m_screenManager; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IAudioManager* EngineInstance::audioManager() const 
{ 
  return m_audioManager; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Debug* EngineInstance::debug() const 
{ 
  return m_debug; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DeviceServices* EngineInstance::deviceServices() const 
{ 
  return m_deviceServices; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PurchaseServices* EngineInstance::purchaseServices() const
{
  return m_purchaseServices;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SocialServices* EngineInstance::socialServices() const
{
  return m_socialServices;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImageLoader* EngineInstance::imageLoader() const 
{ 
  return m_imageLoader; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void EngineInstance::setLanguage(const String& language)
{
  if (language != m_language)
  {
    m_language = language;

    // broadcast change
    eventManager()->send(EGE_EVENT_ID_CORE_LANGUAGE_CHANGED);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& EngineInstance::language() const
{
  return m_language;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AdNetworkRegistry* EngineInstance::adNetworkRegistry() const
{
  return m_adNetworkRegistry;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void EngineInstance::onEventRecieved(PEvent event)
{
  switch (event->id())
  {
    case EGE_EVENT_ID_CORE_QUIT_REQUEST:

      shutdown();
      break;
      
    case EGE_EVENT_ID_CORE_APP_PAUSE:

      EGE_ASSERT(EStateRunning == state());

      m_application->onSuspend();
      m_state = EStatePaused;
      break;

    case EGE_EVENT_ID_CORE_APP_RESUME:

      EGE_ASSERT(EStatePaused == state());

      m_application->onResume();
      m_state = EStateRunning;
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EngineState EngineInstance::state() const
{
  return m_state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END