#include "Core/Application/Application.h"
#include "Core/Graphics/Graphics.h"
#include "Core/Graphics/Render/RenderTarget.h"
#include "Core/Graphics/Render/RenderSystem.h"
#include "Core/Graphics/Render/RenderWindow.h"
#include "Core/Physics/PhysicsManager.h"
#include "Core/Graphics/Particle/ParticleFactory.h"
#include "Core/UI/WidgetFactory.h"
#include "Core/Data/DataBuffer.h"
#include <EGEDevice.h>
#include <EGEDebug.h>

#include <EGEMutex.h>

#ifdef EGE_PLATFORM_WIN32
#include "Win32/Graphics/GraphicsWin32_p.h"
#elif EGE_PLATFORM_AIRPLAY
#include "Airplay/Graphics/GraphicsAirplay_p.h"
#endif

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Graphics)
EGE_DEFINE_DELETE_OPERATORS(Graphics)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Graphics::Graphics(Application* app, const Dictionary& params) : Object(app),
                                                                 m_p(NULL),
                                                                 m_renderSystem(NULL),
                                                                 m_particleFactory(NULL),
                                                                 m_widgetFactory(NULL),
                                                                 m_renderingEnabled(true),
                                                                 m_nextHandle(0)
{
  m_params = params;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Graphics::~Graphics()
{
  unregisterAllRenderTargets();

  EGE_DELETE(m_renderSystem);
  EGE_DELETE(m_particleFactory);
  EGE_DELETE(m_widgetFactory);
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult Graphics::construct()
{
  EGEResult result;

  // create private implementation
  m_p = ege_new GraphicsPrivate(this);
  if (NULL == m_p)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // construct private implementation
  if (EGE_SUCCESS != (result = m_p->construct()))
  {
    // error!
    return result;
  }

  // create render system
  m_renderSystem = ege_new RenderSystem(app());
  if (NULL == m_renderSystem)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  if (EGE_SUCCESS != (result = m_renderSystem->construct()))
  {
    // error!
    return result;
  }

  // create particle factory
  m_particleFactory = ege_new ParticleFactory(app());
  if (NULL == m_particleFactory)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  if (EGE_SUCCESS != (result = m_particleFactory->construct()))
  {
    // error!
    return result;
  }

  // create widget factory
  m_widgetFactory = ege_new WidgetFactory(app());
  if (NULL == m_widgetFactory)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  if (EGE_SUCCESS != (result = m_widgetFactory->construct()))
  {
    // error!
    return result;
  }

  // create access mutex
  m_mutex = ege_new Mutex(app());
  if (NULL == m_mutex)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Graphics::render()
{
  // check if rendering is disabled
  if ( ! m_renderingEnabled)
  {
    // do not render
    return;
  }

  // go thru all elements
  for (RenderTargetMap::const_iterator iter = m_renderTargets.begin(); iter != m_renderTargets.end(); ++iter)
  {
    PRenderTarget target = iter->second;

    // check if enabled
    if (target->isEnabled())
    {
      emit preRender(target);

      // render target
      target->render();

      emit postRender(target);
    }
  }

  // render physics data
  app()->physicsManager()->render();

  // show what has been rendered in render window
  PRenderWindow window = renderTarget(EGE_PRIMARY_RENDER_TARGET_NAME);
  window->showFrameBuffer();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Graphics::registerRenderTarget(PRenderTarget target)
{
  m_renderTargets.insert(target->priority(), target);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Graphics::removeRenderTarget(const String& name)
{
  // go thru all elements
  for (RenderTargetMap::iterator iter = m_renderTargets.begin(); iter != m_renderTargets.end(); ++iter)
  {
    // check if given render target has been found
    if (name == iter->second->name())
    {
      // deallocate
      iter->second = NULL;

      // remove from pool
      m_renderTargets.erase(iter);
      break;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PRenderTarget Graphics::renderTarget(const String& name) const
{
  // go thru all elements
  for (RenderTargetMap::const_iterator iter = m_renderTargets.begin(); iter != m_renderTargets.end(); ++iter)
  {
    // check if given render target has been found
    if (name == iter->second->name())
    {
      return iter->second;
    }
  }

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Graphics::unregisterAllRenderTargets()
{
  // go thru all elements
  for (RenderTargetMap::iterator iter = m_renderTargets.begin(); iter != m_renderTargets.end();)
  {
    // deallocate
    iter->second = NULL;

    // remove from pool
    m_renderTargets.erase(iter++);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Graphics::setRenderingEnabled(bool set)
{
  m_renderingEnabled = set;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Graphics::initializeWorkThreadRenderingContext()
{
  p_func()->initializeWorkThreadRenderingContext();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Graphics::deinitializeWorkThreadRenderingContext()
{
  p_func()->deinitializeWorkThreadRenderingContext();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 Graphics::createTexture2D(const PImage& image)
{
  TextureCreateRequest request;
  request.image  = image;
  request.handle = m_nextHandle++;

  // add to pool
  MutexLocker locker(m_mutex);
  m_texture2DRequests.push_back(request);

  return request.handle;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Graphics::update()
{
  MutexLocker locker(m_mutex);
  for (TextureCreateRequestList::iterator it = m_texture2DRequests.begin(); it != m_texture2DRequests.end(); ++it)
  {
    TextureCreateRequest& request = *it;

    PTexture2D texture = renderSystem()->createTexture2D("ala", request.image);
 
    emit texture2DCreated(request.handle, texture, EGE_SUCCESS);
  }
  m_texture2DRequests.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IRenderer* Graphics::renderer()
{
  return m_renderSystem;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END