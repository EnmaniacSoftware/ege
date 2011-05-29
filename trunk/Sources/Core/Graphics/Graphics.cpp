#include "Core/Application/Application.h"
#include "Core/Graphics/Graphics.h"
#include "Core/Graphics/Render/RenderTarget.h"
#include "Core/Graphics/Render/Renderer.h"
#include "Core/Graphics/Render/RenderWindow.h"
#include "Core/Physics/PhysicsManager.h"
#include "EGEDevice.h"

#ifdef EGE_PLATFORM_WIN32
#include "Win32/Graphics/GraphicsWin32_p.h"
#elif EGE_PLATFORM_AIRPLAY
#include "Airplay/Graphics/GraphicsAirplay_p.h"
#endif

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(Graphics)
EGE_DEFINE_DELETE_OPERATORS(Graphics)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Graphics::Graphics(Application* app, const ConfigParams& params) : Object(app)
{
  m_p = ege_new GraphicsPrivate(this, params);

  // find primary render surface
  PRenderWindow window = renderTarget(EGE_PRIMARY_RENDER_TARGET_NAME);

  // make primary render target currently selected
  setCurrentRenderingContext(window);

  // create renderer
  m_renderer = ege_new Renderer(app);

  // determine orientation rotation

  // check if landscape mode is requested
  if (app->isLandscape())
  {
    // check if main surface in portrait mode
    if (Device::SurfaceWidth() < Device::SurfaceHeight())
    {
      m_renderer->setOrientationRotation(Angle::FromDegrees(270));
    }
  }
  else
  {
    // check if main surface in landscape mode
    if (Device::SurfaceWidth() > Device::SurfaceHeight())
    {
      m_renderer->setOrientationRotation(Angle::FromDegrees(-270));
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Graphics::~Graphics()
{
  // release any rendering context
  setCurrentRenderingContext(NULL);

  unregisterAllRenderTargets();

  m_renderer = NULL;

  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Renders all registered targets. */
void Graphics::render()
{
  // go thru all elements
  for (EGEMultiMap<s32, PRenderTarget>::const_iterator iter = m_renderTargets.begin(); iter != m_renderTargets.end(); ++iter)
  {
    PRenderTarget target = iter->second;

    // render target
    target->render();
  }

  // render physics data
  app()->physicsManager()->render();

  // show what has been rendered in render window
  PRenderWindow window = renderTarget(EGE_PRIMARY_RENDER_TARGET_NAME);
  window->showFrameBuffer();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets current rendering context. */
EGEResult Graphics::setCurrentRenderingContext(PRenderTarget pRenderTarget)
{
  EGEResult eResult = EGE_SUCCESS;

  // check if there is any rendering context in use atm
  if (m_currentRenderingContext)
  {
    // release it
    m_currentRenderingContext->releaseCurrentContext();
  }

  // set new rendering context
  m_currentRenderingContext = pRenderTarget;

  // make it current if valid
  if (m_currentRenderingContext)
  {
    eResult = m_currentRenderingContext->makeCurrentContext();
  }

  return eResult;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool Graphics::isValid() const
{
  return (NULL != m_p) && (NULL != m_renderer);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Graphics::registerRenderTarget(PRenderTarget target)
{
  m_renderTargets.insert(EGEMultiMap<s32, PRenderTarget>::value_type(target->priority(), target));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Graphics::removeRenderTarget(const EGEString& name)
{
  // go thru all elements
  for (EGEMultiMap<s32, PRenderTarget>::iterator iter = m_renderTargets.begin(); iter != m_renderTargets.end(); ++iter)
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
PRenderTarget Graphics::renderTarget(const EGEString& name) const
{
  // go thru all elements
  for (EGEMultiMap<s32, PRenderTarget>::const_iterator iter = m_renderTargets.begin(); iter != m_renderTargets.end(); ++iter)
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
  for (EGEMultiMap<s32, PRenderTarget>::iterator iter = m_renderTargets.begin(); iter != m_renderTargets.end();)
  {
    // deallocate
    iter->second = NULL;

    // remove from pool
    m_renderTargets.erase(iter++);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
