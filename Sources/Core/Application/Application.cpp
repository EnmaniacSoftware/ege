#include "Core/Application/Application.h"
#include "Core/Graphics/Viewport.h"
#include "Core/Graphics/Camera.h"
#include "Core/Graphics/Render/RenderWindow.h"
#include "Core/Graphics/Render/RenderTarget.h"
#include "Core/Graphics/Render/RenderSystem.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Physics/PhysicsManager.h"
#include "Core/Overlay/OverlayManager.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Event/EventManager.h"
#include "Core/ComplexTypes.h"
#include "Core/Event/Event.h"
#include "Core/Input/Pointer.h"
#include "Core/Screen/ScreenManager.h"
#include "Core/Audio/AudioManager.h"
#include "Core/Audio/Null/AudioManagerNull.h"
#include "Core/Graphics/Image/ImageLoader.h"
#include "EGEDebug.h"
#include "EGEDeviceServices.h"
#include "EGEPurchaseServices.h"
#include "EGESocialServices.h"
#include "EGETimer.h"
#include "EGEDirectory.h"
#include "EGEResources.h"

#ifdef EGE_PLATFORM_WIN32
  #include "Win32/Application/ApplicationWin32_p.h"
  #include "Core/Audio/OpenAL/AudioManagerOpenAL.h"
#elif EGE_PLATFORM_AIRPLAY
  #include "Airplay/Application/ApplicationAirplay_p.h"
  #include "Airplay/Audio/AudioManagerAirplay.h"
#elif EGE_PLATFORM_IOS
  #include "iOS/Application/ApplicationIOS_p.h"
  #include "iOS/Audio/OpenAL/AudioManagerOpenALIOS.h"
#endif // EGE_PLATFORM_WIN32

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* KApplicationDebugName = "EGEApplication";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Application::Application() : IEventListener(),
                             m_p(NULL),
                             m_sceneManager(NULL), 
                             m_physicsManager(NULL), 
                             m_eventManager(NULL), 
                             m_graphics(NULL), 
                             m_resourceManager(NULL), 
                             m_pointer(NULL), 
                             m_overlayManager(NULL), 
                             m_screenManager(NULL), 
                             m_audioManager(NULL),
                             m_debug(NULL), 
                             m_deviceServices(NULL),
                             m_purchaseServices(NULL),
                             m_socialServices(NULL),
                             m_imageLoader(NULL),
                             m_landscapeMode(false),
                             m_language("en"),
                             m_updateInterval(0LL),
                             m_renderInterval(0LL),
                             m_state(STATE_INVALID)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Application::~Application()
{
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

  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult Application::construct(const Dictionary& params)
{
  EGEResult result;

  // decompose param list
  Dictionary::const_iterator iterLandscape  = params.find(EGE_ENGINE_PARAM_LANDSCAPE_MODE);
  Dictionary::const_iterator iterUPS        = params.find(EGE_ENGINE_PARAM_UPDATES_PER_SECOND);
  Dictionary::const_iterator iterFPS        = params.find(EGE_ENGINE_PARAM_RENDERS_PER_SECOND);

  // check if update rate is given
  if (iterUPS != params.end())
  {
    m_updateInterval.fromMiliseconds(1000 / iterUPS->second.toInt());
  }

  // check if render rate is given
  if (iterFPS != params.end())
  {
    m_renderInterval.fromMiliseconds(1000 / iterFPS->second.toInt());
  }

  // check landscape mode
  if (iterLandscape != params.end())
  {
    m_landscapeMode = iterLandscape->second.toBool();
  }
  
  // load configuration
  loadConfig();

  // create private implementation
  m_p = ege_new ApplicationPrivate(this);
  if (NULL == m_p)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // create device services
  m_deviceServices = ege_new PLATFORM_CLASSNAME(DeviceServices)();
  if (NULL == m_deviceServices)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // create purchase services
  m_purchaseServices = ege_new PLATFORM_CLASSNAME(PurchaseServices)(this);
  if (NULL == m_purchaseServices)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // create social services
  m_socialServices = ege_new PLATFORM_CLASSNAME(SocialServices)(this);
  if (NULL == m_socialServices)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // create event manager
  m_eventManager = ege_new EventManager(this);
  if (NULL == m_eventManager)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  if (EGE_SUCCESS != (result = m_eventManager->construct()))
  {
    // error!
    return result;
  }

  // create image loader
  m_imageLoader = ege_new ImageLoader(this);
  if (NULL == m_imageLoader)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  if (EGE_SUCCESS != (result = m_imageLoader->construct()))
  {
    // error!
    return result;
  }

  // create graphics
  // NOTE: must be before ResourceManager
  m_graphics = ege_new Graphics(this, params);
  if (NULL == m_graphics)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  if (EGE_SUCCESS != (result = m_graphics->construct()))
  {
    // error!
    return result;
  }

  // create resource manager
  m_resourceManager = ege_new ResourceManager(this);
  if (NULL == m_resourceManager)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  if (EGE_SUCCESS != (result = m_resourceManager->construct()))
  {
    // error!
    return result;
  }

  // create physics manager
  m_physicsManager = ege_new PhysicsManager(this);
  if (NULL == m_physicsManager)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  if (EGE_SUCCESS != (result = m_physicsManager->construct(params)))
  {
    // error!
    return result;
  }

  // create scene manager
  m_sceneManager = ege_new SceneManager(this);
  if (NULL == m_sceneManager)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  if (EGE_SUCCESS != (result = m_sceneManager->construct()))
  {
    // error!
    return result;
  }

  // create overlay manager
  m_overlayManager = ege_new OverlayManager(this);
  if (NULL == m_overlayManager)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  if (EGE_SUCCESS != (result = m_overlayManager->construct()))
  {
    // error!
    return result;
  }

  // create pointer input
  m_pointer = ege_new Pointer(this);
  if (NULL == m_pointer)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  if (EGE_SUCCESS != (result = m_pointer->construct()))
  {
    // error!
    return result;
  }

  // create screen manager
  m_screenManager = ege_new ScreenManager(this);
  if (NULL == m_screenManager)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  if (EGE_SUCCESS != (result = m_screenManager->construct()))
  {
    // error!
    return result;
  }

  // create audio manager
  // TAGE - make a decision based on some data, to figure out yet
#if EGE_AUDIO_NULL
  m_audioManager = ege_new AudioManagerNull(this);
#elif EGE_AUDIO_OPENAL
  #if EGE_PLATFORM_IOS
    m_audioManager = ege_new AudioManagerOpenALIOS(this);
  #else
    m_audioManager = ege_new AudioManagerOpenAL(this);
  #endif // EGE_PLATFORM_IOS
#else
  m_audioManager = ege_new AudioManagerAirplay(this);
#endif // EGE_AUDIO_NULL

  if (EGE_SUCCESS != (result = m_audioManager->construct()))
  {
    // error!
    return result;
  }

  // subscribe for event notifications
  if ( ! eventManager()->addListener(this))
  {
    // error!
    return EGE_ERROR;
  }

  // connect for notifications
  ege_connect(resourceManager(), groupLoadComplete, this, Application::onGroupLoadComplete);
  ege_connect(resourceManager(), groupLoadError, this, Application::onGroupLoadError);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Application::update()
{ 
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
  if (STATE_QUITTING == m_state)
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
      m_state = STATE_QUIT;
    }
  }
  else if (STATE_RUNNING == m_state)
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

    update(timeInterval);

    // interpolate physics by remaining value
    // ..
  }

  // store update duration
  m_lastFrameUpdateDuration.fromMicroseconds(Timer::GetMicroseconds() - m_lastUpdateTime.microseconds());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Application::update(const Time& time)
{
  EGE_UNUSED(time);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Application::render()
{
  if (STATE_RUNNING == m_state)
  {
    // do render
    graphics()->render();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult Application::run()
{
  // initialize update timer (to smooth out first update)
  if (0 == m_lastUpdateTime.microseconds())
  {
    m_lastUpdateTime.fromMicroseconds(Timer::GetMicroseconds());

    // set state
    m_state = STATE_RUNNING;
  }

  return p_func()->run();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Application::setLanguage(const String& language)
{
  if (language != m_language)
  {
    m_language = language;

    // broadcast change
    eventManager()->send(EGE_EVENT_ID_CORE_LANGUAGE_CHANGED);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Application::quit()
{
  eventManager()->send(EGE_EVENT_ID_CORE_QUIT_REQUEST);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Application::isQuitting() const
{
  return (STATE_QUIT == state()) || (STATE_QUITTING == state());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Application::onEventRecieved(PEvent event)
{
  switch (event->id())
  {
    case EGE_EVENT_ID_CORE_QUIT_REQUEST:

      m_state = STATE_QUITTING;
      break;
      
    case EGE_EVENT_ID_CORE_APP_PAUSE:

      if (STATE_RUNNING == state())
      {
        m_state = STATE_PAUSED;
      }
      break;

    case EGE_EVENT_ID_CORE_APP_RESUME:

      if (STATE_PAUSED == state())
      {
        m_state = STATE_RUNNING;
      }
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Application::loadConfig()
{
  const char* KConfigFileName     = "config.xml";
  const char* KRootElement        = "config";
  const char* KLogElement         = "log";
  const char* KLogNameAttribute   = "name";
  const char* KLogEnableAttribute = "enabled";

  StringList enabledDebugNames;

  // compose path to potential config file
  String configPath = Directory::Join(Directory::Path(Directory::EDocuments), String(KConfigFileName));

  XmlDocument xml;
  if (EGE_SUCCESS == xml.load(configPath))
  {
    // find the root element
    PXmlElement root = xml.firstChild(KRootElement);
    if ((NULL != root) && root->isValid())
    {
      // go thru all children and read as much as we can
      PXmlElement child = root->firstChild();
      while (child->isValid())
      {
        // get child name
        const String childName = child->name();

        // check if LOG node
        if (KLogElement == childName)
        {
          // process data
          String sectionName = child->attribute(KLogNameAttribute, "");
          bool enabled       = child->attribute(KLogEnableAttribute, false);

          // check if section is enabled
          if (enabled)
          {
            // add to pool
            enabledDebugNames << sectionName;
          }
        }
  
        // go to next child
        child = child->nextChild();
      }
    }
  }

  // enable debug info
  Debug::EnableNames(enabledDebugNames);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Application::onGroupLoadComplete(const String& name)
{
  if (DEFAULT_GROUP_NAME == name)
  {
    onInitialized();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Application::onGroupLoadError(const String& name)
{
  if (DEFAULT_GROUP_NAME == name)
  {
    quit();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END