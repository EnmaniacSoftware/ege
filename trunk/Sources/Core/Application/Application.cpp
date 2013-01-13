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
#include "Core/Tools/DesktopServices.h"
#include "Core/Audio/AudioManager.h"
#include "Core/Graphics/Image/ImageLoader.h"
#include <EGETimer.h>
#include <EGEGraphics.h>
#include <EGEApplication.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Application::Application() : m_sceneManager(NULL), 
                             m_physicsManager(NULL), 
                             m_eventManager(NULL), 
                             m_graphics(NULL), 
                             m_appController(NULL), 
                             m_resourceManager(NULL), 
                             m_pointer(NULL), 
                             m_overlayManager(NULL), 
                             m_screenManager(NULL), 
                             m_audioManager(NULL),
                             m_debug(NULL), 
                             m_desktopServices(NULL),
                             m_imageLoader(NULL),
                             m_landscapeMode(false),
                             m_language("en")
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Application::~Application()
{
  EGE_DELETE(m_imageLoader);
  EGE_DELETE(m_sceneManager);
  EGE_DELETE(m_imageLoader);
  EGE_DELETE(m_graphics);
  EGE_DELETE(m_appController);
  EGE_DELETE(m_screenManager);
  EGE_DELETE(m_audioManager);
  EGE_DELETE(m_overlayManager);
  EGE_DELETE(m_pointer);
  EGE_DELETE(m_eventManager);
  EGE_DELETE(m_physicsManager);
  EGE_DELETE(m_desktopServices);
  EGE_DELETE(m_resourceManager);
  //EGE_DELETE(m_debug);

  MemoryManager::Deinit();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult Application::initialize(const Dictionary& params)
{
  EGEResult result;

  // check landscape mode
  m_landscapeMode = false;
  Dictionary::const_iterator iterLandscape = params.find(EGE_ENGINE_PARAM_LANDSCAPE_MODE);
  if (iterLandscape != params.end())
  {
    m_landscapeMode = iterLandscape->second.toBool();
  }
  
  // create desktop services
  m_desktopServices = ege_new DesktopServices();
  if (NULL == m_desktopServices)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  if (EGE_SUCCESS != (result = m_desktopServices->construct()))
  {
    // error!
    return result;
  }

  // create event manager
  m_eventManager = ege_new EventManager(this);
  if (NULL == m_eventManager)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
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

  // create application controller
  m_appController = ege_new AppController(this, params);
  if (NULL == m_appController)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  if (EGE_SUCCESS != (result = m_appController->construct()))
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
  m_audioManager = ege_new AudioManager(this);
  if (NULL == m_audioManager)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  if (EGE_SUCCESS != (result = m_audioManager->construct()))
  {
    // error!
    return result;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult Application::run()
{
  return controller()->run();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Application::update(const Time& time)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 Application::fps() const
{
  return controller()->fps();
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
  return (AppController::STATE_QUIT == controller()->state()) || (AppController::STATE_QUITTING == controller()->state());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const Time& Application::lastFrameUpdateDuration() const
{
  return controller()->lastFrameUpdateDuration();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const Time& Application::lastFrameRenderDuration() const
{
  return controller()->lastFrameRenderDuration();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END