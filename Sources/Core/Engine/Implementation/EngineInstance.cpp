#include "Core/Engine/Interface/EngineInstance.h"
#include "Core/Graphics/Graphics.h"
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
#include "Core/Screen/ScreenManager.h"
#include "Core/Audio/Interface/AudioManager.h"
#include "Core/Graphics/Image/ImageLoader.h"
#include "Core/Services/Interface/AdNetworkRegistry.h"
#include "Core/Services/Interface/SocialServicesNull.h"
#include "Core/Services/Interface/AdNetwork.h"
#include "Core/Event/Implementation/EventManager.h"
#include "EGEInput.h"
#include "EGEEvent.h"
#include "EGEApplication.h"
#include "EGEDeviceServices.h"
#include "EGEPurchaseServices.h"
#include "EGESocialServices.h"
#include "EGETimer.h"
#include "EGEDirectory.h"
#include "EGEResources.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* KEngineDebugName = "EGEEngine";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(EngineInstance)
EGE_DEFINE_DELETE_OPERATORS(EngineInstance)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EngineInstance::EngineInstance()
: m_socialServices(NULL)
, m_state(EStateInvalid)
, m_application(NULL)
, m_pointer(NULL)
, m_debug(NULL)
, m_deviceServices(NULL)
, m_purchaseServices(NULL)
, m_imageLoader(NULL)
, m_adNetworkRegistry(NULL)
, m_audioManagerFactory(NULL)
, m_adNetwork(NULL)
, m_lastUpdateTime(0LL)
, m_updateAccumulator(0LL)
, m_language("en")
{
  m_adNetworkRegistry = ege_new AdNetworkRegistry(*this);
  m_audioManagerFactory = ege_new AudioManagerFactory(*this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EngineInstance::~EngineInstance()
{
  EngineApplication::DestroyInstance(m_application);
  m_application = NULL;

  EGE_DELETE(m_audioManagerFactory);
  EGE_DELETE(m_adNetwork);
  EGE_DELETE(m_adNetworkRegistry);
  EGE_DELETE(m_imageLoader);
  EGE_DELETE(m_pointer);
  EGE_DELETE(m_deviceServices);
  EGE_DELETE(m_purchaseServices);
  EGE_DELETE(m_socialServices);

  for (EngineModulesPool::reverse_iterator it = m_modules.rbegin(); it != m_modules.rend(); ++it)
  {
    delete it->second;
  }
  m_modules.clear();
  m_modulesUIDToObjects.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const Dictionary& EngineInstance::configurationDictionary() const
{
  return m_configurationDictionary;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult EngineInstance::construct(const Dictionary& commandLineDictionary)
{
  EGEResult result;

  String typeName;

  // create application instance
  m_application = EngineApplication::CreateInstance(*this);

  // retrieve engine configuration
  m_configurationDictionary = commandLineDictionary;
  m_configurationDictionary.merge(m_application->engineConfiguration());

  // decompose param list
  const s32 updatesPerSecond = m_configurationDictionary.value(EGE_ENGINE_PARAM_UPDATES_PER_SECOND, "0").toInt();
  const s32 framesPerSecond  = m_configurationDictionary.value(EGE_ENGINE_PARAM_RENDERS_PER_SECOND, "0").toInt();

  // check if update rate is given
  if (0 < updatesPerSecond)
  {
    m_updateInterval.fromMiliseconds(1000 / updatesPerSecond);
  }

  // check if render rate is given
  if (0 < framesPerSecond)
  {
    m_renderInterval.fromMiliseconds(1000 / framesPerSecond);
  }
  
  // load configuration
  loadConfig();

  // create modules
  createModules();

  // construct
  if (EGE_SUCCESS != (result = constructModules()))
  {
    // error!
    return result;
  }

  // subscribe for event notifications
  if ( ! eventManager()->registerListener(this))
  {
    // error!
    return EGE_ERROR;
  }

  // set state
  m_state = EStatePaused;

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
  
  // update accumulator for updates
  m_updateAccumulator += timeInterval;

  // update next render intervals
  m_nextRenderTimeLeft -= timeInterval;

  // update as much as requested
  bool canClose = false;
  while (m_updateAccumulator >= m_updateInterval)
  {
    canClose = true;

    m_updateAccumulator -= m_updateInterval;

    // process all modules
    for (EngineModulesPool::iterator it = m_modules.begin(); it != m_modules.end(); ++it)
    {
      IEngineModule* module = it->second;

      // update
      module->update(m_updateInterval);

      // check if can close
      canClose &= (EModuleStateClosed == module->state());
    }

    // TAGE - make module ?
    m_application->update(m_updateInterval);
  }

  // check if quitting
  if ((EStateQuitting == state()) && canClose)
  {
    // change state
    m_state = EStateClosed;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void EngineInstance::render()
{
  // check if can render
  if (0 >= m_nextRenderTimeLeft.microseconds())
  {
    m_nextRenderTimeLeft += m_renderInterval;

    if (EStateRunning == state())
    {
      // do render
      graphics()->render();
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void EngineInstance::shutdown()
{
  if ((EStateQuitting != m_state) && (EStateClosed != m_state))
  {
    // notify modules
    for (EngineModulesPool::iterator it = m_modules.begin(); it != m_modules.end(); ++it)
    {
      IEngineModule* module = it->second;

      module->onShutdown();
    }

    // set state
    m_state = EStateQuitting;
  }
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
IGraphics* EngineInstance::graphics() const 
{ 
  EGE_ASSERT(m_modulesUIDToObjects.contains(EGE_OBJECT_UID_GRAPHICS_MODULE));
  return static_cast<EngineModule<IGraphics>*>(m_modulesUIDToObjects.value(EGE_OBJECT_UID_GRAPHICS_MODULE, NULL))->iface(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IEventManager* EngineInstance::eventManager() const 
{ 
  EGE_ASSERT(m_modulesUIDToObjects.contains(EGE_OBJECT_UID_EVENT_MANAGER_MODULE));
  return static_cast<EngineModule<IEventManager>*>(m_modulesUIDToObjects.value(EGE_OBJECT_UID_EVENT_MANAGER_MODULE, NULL))->iface(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IPhysicsManager* EngineInstance::physicsManager() const 
{ 
  EGE_ASSERT(m_modulesUIDToObjects.contains(EGE_OBJECT_UID_PHYSICS_MANAGER_MODULE));
  return static_cast<EngineModule<IPhysicsManager>*>(m_modulesUIDToObjects.value(EGE_OBJECT_UID_PHYSICS_MANAGER_MODULE, NULL))->iface(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISceneManager* EngineInstance::sceneManager() const 
{ 
  EGE_ASSERT(m_modulesUIDToObjects.contains(EGE_OBJECT_UID_SCENE_MANAGER_MODULE));
  return static_cast<EngineModule<ISceneManager>*>(m_modulesUIDToObjects.value(EGE_OBJECT_UID_SCENE_MANAGER_MODULE, NULL))->iface(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IResourceManager* EngineInstance::resourceManager() const 
{ 
  EGE_ASSERT(m_modulesUIDToObjects.contains(EGE_OBJECT_UID_RESOURCE_MANAGER_MODULE));
  return static_cast<EngineModule<IResourceManager>*>(m_modulesUIDToObjects.value(EGE_OBJECT_UID_RESOURCE_MANAGER_MODULE, NULL))->iface(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Pointer* EngineInstance::pointer() const 
{ 
  return m_pointer; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IOverlayManager* EngineInstance::overlayManager() const 
{ 
  EGE_ASSERT(m_modulesUIDToObjects.contains(EGE_OBJECT_UID_OVERLAY_MANAGER_MODULE));
  return static_cast<EngineModule<IOverlayManager>*>(m_modulesUIDToObjects.value(EGE_OBJECT_UID_OVERLAY_MANAGER_MODULE, NULL))->iface(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IScreenManager* EngineInstance::screenManager() const 
{ 
  EGE_ASSERT(m_modulesUIDToObjects.contains(EGE_OBJECT_UID_SCREEN_MANAGER_MODULE));
  return static_cast<EngineModule<IScreenManager>*>(m_modulesUIDToObjects.value(EGE_OBJECT_UID_SCREEN_MANAGER_MODULE, NULL))->iface(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IAudioManager* EngineInstance::audioManager() const 
{ 
  EGE_ASSERT(m_modulesUIDToObjects.contains(EGE_OBJECT_UID_AUDIO_MANAGER_MODULE));
  return static_cast<EngineModule<IAudioManager>*>(m_modulesUIDToObjects.value(EGE_OBJECT_UID_AUDIO_MANAGER_MODULE, NULL))->iface(); 
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
AudioManagerFactory* EngineInstance::audioManagerFactory() const
{
  return m_audioManagerFactory;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void EngineInstance::onEventRecieved(PEvent event)
{
  switch (event->id())
  {
    case EGE_EVENT_ID_CORE_QUIT_REQUEST:

      egeDebug(KEngineDebugName) << "QUIT request received";
      shutdown();
      break;
      
    case EGE_EVENT_ID_CORE_APP_PAUSE:

      if (EStateRunning == state())
      {
        egeDebug(KEngineDebugName) << "SUSPEND request received";

        m_application->onSuspend();

        // notify modules
        for (EngineModulesPool::iterator it = m_modules.begin(); it != m_modules.end(); ++it)
        {
          IEngineModule* module = it->second;

          module->onSuspend();
        }

        m_state = EStatePaused;
      }
      break;

    case EGE_EVENT_ID_CORE_APP_RESUME:

      if (EStatePaused == state())
      {
        egeDebug(KEngineDebugName) << "RESUME request received";

        // notify modules
        for (EngineModulesPool::iterator it = m_modules.begin(); it != m_modules.end(); ++it)
        {
          IEngineModule* module = it->second;

          module->onResume();
        }

        m_application->onResume();

        m_state = EStateRunning;
      }
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EngineState EngineInstance::state() const
{
  return m_state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AdNetwork* EngineInstance::adNetwork() const
{
  return m_adNetwork;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void EngineInstance::loadConfig()
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
void EngineInstance::createModules()
{
  String typeName;

  const u32 KModulePriorityEventManager     = 0;
  const u32 KModulePriorityGraphics         = 5;
  const u32 KModulePriorityResourceManager  = 10;
  const u32 KModulePriorityPhysicsManager   = 15;
  const u32 KModulePrioritySceneManager     = 20;
  const u32 KModulePriorityOverlayManager   = 25;
  const u32 KModulePriorityScreenManager    = 30;
  const u32 KModulePriorityAudioManager     = 35;

  // create device services
  m_deviceServices = ege_new PLATFORM_CLASSNAME(DeviceServices)(*this);

  // create purchase services
  m_purchaseServices = ege_new PLATFORM_CLASSNAME(PurchaseServices)(*this);

  // create social services if not created yet
  if (NULL == m_socialServices)
  {
    m_socialServices = ege_new SocialServicesNull(*this);
  }

  IEngineModule* module;

  // create event manager
  module = ege_new EventManager();
  m_modules.insert(KModulePriorityEventManager, module);
  m_modulesUIDToObjects.insert(module->uid(), module);

  // create image loader
  m_imageLoader = ege_new ImageLoader(*this);

  // create graphics
  // NOTE: must be before ResourceManager
  module = ege_new Graphics(*this, m_configurationDictionary);
  m_modules.insert(KModulePriorityGraphics, module);
  m_modulesUIDToObjects.insert(module->uid(), module);

  // create resource manager
  module = ege_new ResourceManager(*this);
  m_modules.insert(KModulePriorityResourceManager, module);
  m_modulesUIDToObjects.insert(module->uid(), module);

  // create physics manager
  module = ege_new PhysicsManager(m_configurationDictionary);
  m_modules.insert(KModulePriorityPhysicsManager, module);
  m_modulesUIDToObjects.insert(module->uid(), module);

  // create scene manager
  module = ege_new SceneManager(*this);
  m_modules.insert(KModulePrioritySceneManager, module);
  m_modulesUIDToObjects.insert(module->uid(), module);

  // create overlay manager
  module = ege_new OverlayManager();
  m_modules.insert(KModulePriorityOverlayManager, module);
  m_modulesUIDToObjects.insert(module->uid(), module);

  // create pointer input
  m_pointer = ege_new Pointer(*this);

  // create screen manager
  module = ege_new ScreenManager(*this);
  m_modules.insert(KModulePriorityScreenManager, module);
  m_modulesUIDToObjects.insert(module->uid(), module);

  // create audio manager
  typeName = configurationDictionary().value(KConfigParamAudioManagerTypeName, KDefaultAudioManagerName);
  module = audioManagerFactory()->createInstance(typeName);
  m_modules.insert(KModulePriorityAudioManager, module);
  m_modulesUIDToObjects.insert(module->uid(), module);

  // create ad network instance
  typeName = configurationDictionary().value(KConfigParamAdNetworkTypeName, KDefaultAdNetworkName);
  m_adNetwork = adNetworkRegistry()->createInstance(typeName);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult EngineInstance::constructModules()
{
  EGEResult result = EGE_SUCCESS;

  // construct modules
  for (EngineModulesPool::iterator it = m_modules.begin(); (it != m_modules.end()) && (EGE_SUCCESS == result); ++it)
  {
    IEngineModule* module = it->second;

    result = module->construct();
  }

  //if (EGE_SUCCESS != (result = m_adNetwork->construct()))
  //{
  //  // error!
  //  return result;
  //}

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END