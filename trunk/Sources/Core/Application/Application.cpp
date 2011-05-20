#include "Core/Graphics/Render/RenderWindow.h"
#include "Core/Graphics/Render/RenderTarget.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Physics/PhysicsManager.h"
#include "Core/Overlay/OverlayManager.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Event/EventManager.h"
#include "Core/Graphics/Viewport.h"
#include "Core/Graphics/Camera.h"
#include "Core/Debug/Debug.h"
#include "Core/ComplexTypes.h"
#include "Core/Event/Event.h"
#include "Core/Input/Pointer.h"
#include "EGETimer.h"
#include "EGEGraphics.h"
#include "EGEApplication.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Application::Application() : m_landscapeMode(false)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Application::~Application()
{
  m_sceneManager    = NULL;
  m_overlayManager  = NULL;
  m_graphics        = NULL;
  m_appController   = NULL;
  m_timer           = NULL;
  m_physicsManager  = NULL;
  m_resourceManager = NULL;
  m_pointer         = NULL;
  m_eventManager    = NULL;

  Debug::Deinit();
  MemoryManager::Deinit();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/** Initializes engine.
*  
*   \param  params    list of parameters to initialize engine with
*   \param  listener  listener object which is to be notified with engine events. 
*/
EGEResult Application::initialize(const ConfigParams& params)
{
  EGEResult eResult = EGE_SUCCESS;

  // check landscape mode
  m_landscapeMode = false;
  ConfigParams::const_iterator iterLandscape = params.find(EGE_ENGINE_PARAM_LANDSCAPE_MODE);
  if (iterLandscape != params.end())
  {
    m_landscapeMode = iterLandscape->second.toBool();
  }
 
  // create event manager
  m_eventManager = ege_new EventManager(this);
  if (NULL == m_eventManager)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // create timer
  m_timer = ege_new Timer(this);
  if (NULL == m_timer || !m_timer->isValid())
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // create resource manager
  m_resourceManager = ege_new ResourceManager(this);
  if (NULL == m_resourceManager || !m_resourceManager->isValid())
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // create physics manager
  m_physicsManager = ege_new PhysicsManager(this);
  if (NULL == m_physicsManager || !m_physicsManager->isValid())
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // create scene manager
  m_sceneManager = ege_new SceneManager(this);
  if (NULL == m_sceneManager || !m_sceneManager->isValid())
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // create overlay manager
  m_overlayManager = ege_new OverlayManager(this);
  if (NULL == m_overlayManager)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // create graphics
  m_graphics = ege_new Graphics(this, params);
  if (NULL == m_graphics || !m_graphics->isValid())
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // create application controller
  m_appController = ege_new AppController(this, params);
  if (NULL == m_appController || !m_appController->isValid())
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // create pointer input
  m_pointer = ege_new Pointer(this);
  if (NULL == m_pointer || !m_pointer->isValid())
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // register self for event recieval
  m_eventManager->addListener(this);

  return eResult;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns graphics subsystem object. */
PGraphics Application::graphics() const
{
  return m_graphics;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Starts engine work. */
EGEResult Application::run()
{
  return appController()->run();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns event manager. */
PEventManager Application::eventManager() const
{
  return m_eventManager;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns main timer. */
PTimer Application::timer() const
{
  return m_timer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns scene manager. */
PSceneManager Application::sceneManager() const
{
  return m_sceneManager;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns physics manager. */
PPhysicsManager Application::physicsManager() const
{
  return m_physicsManager;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns resource manager. */
PResourceManager Application::resourceManager() const
{
  return m_resourceManager;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns application controller. */
PAppController Application::appController() const
{
  return m_appController;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IEventListener override. Event reciever. */
void Application::onEventRecieved(PEvent event)
{
  PFloat floatData;

  switch (event->uid())
  {
    case EGE_EVENT_UID_CORE_UPDATE:

      floatData = event->data();
    
      update(floatData->value());
      break;
  }}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Application updater. */
void Application::update(const Time& time)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns pointer input. */
PPointer Application::pointer() const
{
  return m_pointer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns overlay manager. */
POverlayManager Application::overlayManager() const
{
  return m_overlayManager;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
