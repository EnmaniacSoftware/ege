#include "Core/Application/Application.h"
#include "Core/Graphics/Graphics.h"
#include "Core/Graphics/Render/RenderTarget.h"
#include "Core/Graphics/Render/Renderer.h"
#include "Core/Graphics/Render/RenderWindow.h"
#include "Core/Physics/PhysicsManager.h"
#include "Core/Graphics/Particle/ParticleFactory.h"
#include "Core/UI/WidgetFactory.h"
#include "Core/Data/DataBuffer.h"
#include <EGEDevice.h>

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
                                                                 m_particleFactory(NULL),
                                                                 m_widgetFactory(NULL),
                                                                 m_renderingEnabled(true)
{
  m_p = ege_new GraphicsPrivate(this, params);
  if (m_p)
  {
    // find primary render surface
    PRenderWindow window = renderTarget(EGE_PRIMARY_RENDER_TARGET_NAME);

    // make primary render target currently selected
    setCurrentRenderingContext(window);

    // create renderer
    m_renderer = ege_new Renderer(app);

    // create particle factory
    m_particleFactory = ege_new ParticleFactory(app);

    // create widget factory
    m_widgetFactory = ege_new WidgetFactory(app);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Graphics::~Graphics()
{
  // release any rendering context
  setCurrentRenderingContext(NULL);

  unregisterAllRenderTargets();

  m_renderer = NULL;

  EGE_DELETE(m_particleFactory);
  EGE_DELETE(m_widgetFactory);
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Renders all registered targets. */
void Graphics::render()
{
  if (!m_renderingEnabled)
  {
    // error!
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
/*! Sets current rendering context. */
EGEResult Graphics::setCurrentRenderingContext(PRenderTarget target)
{
  EGEResult result = EGE_SUCCESS;

  // check if there is any rendering context in use atm
  if (m_currentRenderingContext)
  {
    // release it
    m_currentRenderingContext->releaseCurrentContext();
  }

  // set new rendering context
  m_currentRenderingContext = target;

  // make it current if valid
  if (m_currentRenderingContext)
  {
    result = m_currentRenderingContext->makeCurrentContext();
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool Graphics::isValid() const
{
  return (NULL != m_p) && (NULL != m_renderer) && (NULL != m_particleFactory);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Registers render target for use. */
void Graphics::registerRenderTarget(PRenderTarget target)
{
  m_renderTargets.insert(target->priority(), target);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes render target with the given name from registered pool. */
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
/*! Returns render target with the given name from registered pool. */
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
/*! Unregisteres all render targets. */
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
/*! Creates vertex buffer obejct. */
PVertexBuffer Graphics::createVertexBuffer(EGEVertexBuffer::UsageType usage) const
{
  if (isValid())
  {
    return p_func()->createVertexBuffer(usage);
  }

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates index buffer obejct. */
PIndexBuffer Graphics::createIndexBuffer(EGEIndexBuffer::UsageType usage) const
{
  if (isValid())
  {
    return p_func()->createIndexBuffer(usage);
  }

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Enables/disables rendering. */
void Graphics::setRenderingEnabled(bool set)
{
  m_renderingEnabled = set;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END