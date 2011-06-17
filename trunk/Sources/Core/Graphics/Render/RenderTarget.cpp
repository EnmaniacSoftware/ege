#include "Core/Graphics/Render/RenderTarget.h"
#include "Core/Graphics/Viewport.h"
#include "Core/Graphics/Camera.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(RenderTarget)
EGE_DEFINE_DELETE_OPERATORS(RenderTarget)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderTarget::RenderTarget(Application* app, const ConfigParams& params) : Object(app), m_vertexCount(0), m_batchCount(0), m_width(0), m_height(0)
{
  // decompose param list
  ConfigParams::const_iterator iterName = params.find(EGE_RENDER_TARGET_PARAM_NAME);
  m_name = (iterName != params.end()) ? iterName->second : "";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderTarget::~RenderTarget()
{
  removeAllViewports();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds new viewport for target associated with given camera. */
PViewport RenderTarget::addViewport(const EGEString& name, PCamera camera)
{
  // check if such viewport is there already
  if (NULL != viewport(name))
  {
    // error, cannot create
    return NULL;
  }

  // create new viewport
  PViewport viewport = ege_new Viewport(app(), name, camera, this);
  if (NULL != viewport)
  {
    m_viewports.push_back(viewport);
  }

  return viewport;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes viewport with the given name from target. */
void RenderTarget::removeViewport(const EGEString& name)
{
  for (EGEList<PViewport>::iterator iter = m_viewports.begin(); iter != m_viewports.end(); ++iter)
  {
    PViewport viewport = *iter;

    if (viewport->name() == name)
    {
      viewport = NULL;

      m_viewports.erase(iter);
      return;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns viewport with the given name associated with target. */
PViewport RenderTarget::viewport(const EGEString& name) const
{
  for (EGEList<PViewport>::const_iterator iter = m_viewports.begin(); iter != m_viewports.end(); ++iter)
  {
    PViewport viewport = *iter;

    if (viewport->name() == name)
    {
      return *iter;
    }
  }

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes all viewport associated with target. */
void RenderTarget::removeAllViewports()
{
  for (EGEList<PViewport>::iterator iter = m_viewports.begin(); iter != m_viewports.end();)
  {
    *iter = NULL;

    iter = m_viewports.erase(iter);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Performs rendering for target. */
void RenderTarget::render()
{
  // reset stats
  m_batchCount    = 0;
  m_vertexCount = 0;

  // go thru all viewports
  for (EGEList<PViewport>::const_iterator iter = m_viewports.begin(); iter != m_viewports.end(); ++iter)
  {
    PViewport viewport = *iter;

    // render viewport
    viewport->render();

    // update stats
    m_batchCount  += viewport->batchCount();
    m_vertexCount += viewport->vertexCount();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Returns TRUE if object is valid. */
bool RenderTarget::isValid() const
{
  return !m_name.empty();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
