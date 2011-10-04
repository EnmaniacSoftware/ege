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
#include "Core/Screen/ScreenManager.h"
#include "Core/Debug/Debug.h"
#include <EGETimer.h>
#include <EGEGraphics.h>
#include <EGEApplication.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Application::Application() : m_sceneManager(NULL), m_physicsManager(NULL), m_eventManager(NULL), m_graphics(NULL), m_appController(NULL), 
                             m_resourceManager(NULL), m_pointer(NULL), m_overlayManager(NULL), m_screenManager(NULL), m_debug(NULL), m_landscapeMode(false)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Application::~Application()
{
  EGE_DELETE(m_sceneManager);
  EGE_DELETE(m_graphics);
  EGE_DELETE(m_appController);
  EGE_DELETE(m_resourceManager);

  EGE_DELETE(m_screenManager);
  EGE_DELETE(m_overlayManager);
  EGE_DELETE(m_pointer);
  EGE_DELETE(m_eventManager);
  EGE_DELETE(m_physicsManager);

  EGE_DELETE(m_debug);

  MemoryManager::Deinit();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/** Initializes engine.
*  
*   \param  params    list of parameters to initialize engine with
*   \param  listener  listener object which is to be notified with engine events. 
*/
EGEResult Application::initialize(const Dictionary& params)
{
  EGEResult eResult = EGE_SUCCESS;

  // check landscape mode
  m_landscapeMode = false;
  Dictionary::const_iterator iterLandscape = params.find(EGE_ENGINE_PARAM_LANDSCAPE_MODE);
  if (iterLandscape != params.end())
  {
    m_landscapeMode = iterLandscape->second.toBool();
  }
  
  // create debug object
  m_debug = ege_new Debug(this);
  if (NULL == m_debug)
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

  // create graphics
  // NOTE: must be before ResourceManager
  m_graphics = ege_new Graphics(this, params);
  if (NULL == m_graphics || !m_graphics->isValid())
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
  m_physicsManager = ege_new PhysicsManager(this, params);
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

  // create screen manager
  m_screenManager = ege_new ScreenManager(this);
  if (NULL == m_screenManager)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  return eResult;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Starts engine work. */
EGEResult Application::run()
{
  return appController()->run();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Application updater. */
void Application::update(const Time& time)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns current FPS indication. */
s32 Application::fps() const
{
  return appController()->fps();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
