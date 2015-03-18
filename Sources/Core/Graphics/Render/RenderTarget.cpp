#include "Core/Graphics/Render/RenderTarget.h"
#include "Core/Graphics/Viewport.h"
#include "Core/Graphics/Camera.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(RenderTarget)
EGE_DEFINE_DELETE_OPERATORS(RenderTarget)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderTarget::RenderTarget(const Dictionary& params) 
: Object()
, m_vertexCount(0)
, m_batchCount(0)
, m_physicalWidth(0)
, m_physicalHeight(0)
, m_width(0)
, m_height(0)
, m_orientationRotation(0.0f)
, m_zoom(1.0f)
, m_enabled(true)
{
  // decompose param list
  m_name   = params.value(EGE_RENDER_TARGET_PARAM_NAME);
  m_width  = params.value(EGE_RENDER_TARGET_PARAM_WIDTH, "0").toInt();
  m_height = params.value(EGE_RENDER_TARGET_PARAM_HEIGHT, "0").toInt();
  m_zoom   = params.value(EGE_RENDER_TARGET_PARAM_ZOOM, "1.0").toFloat();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderTarget::~RenderTarget()
{
  removeAllViewports();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool RenderTarget::isValid() const
{
  return !m_name.isEmpty() && (0 < m_width) && (0 < m_height);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PViewport RenderTarget::addViewport(const String& name, PCamera camera)
{
  // check if such viewport is there already
  if (NULL != viewport(name))
  {
    // error, cannot create
    return NULL;
  }

  // create new viewport
  PViewport viewport = ege_new Viewport(name, camera, this);
  if (NULL != viewport)
  {
    m_viewports.append(viewport);
  }

  return viewport;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderTarget::removeViewport(const String& name)
{
  for (DynamicArray<PViewport>::Iterator iter = m_viewports.begin(); iter != m_viewports.end(); ++iter)
  {
    PViewport viewport = *iter;

    if (viewport->name() == name)
    {
      viewport = NULL;

      m_viewports.remove(iter);
      return;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PViewport RenderTarget::viewport(const String& name) const
{
  for (DynamicArray<PViewport>::ConstIterator iter = m_viewports.begin(); iter != m_viewports.end(); ++iter)
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
void RenderTarget::removeAllViewports()
{
  for (DynamicArray<PViewport>::Iterator iter = m_viewports.begin(); iter != m_viewports.end();)
  {
    *iter = NULL;

    iter = m_viewports.remove(iter);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderTarget::render()
{
  // reset stats
  m_batchCount    = 0;
  m_vertexCount = 0;

  // go thru all viewports
  for (DynamicArray<PViewport>::ConstIterator iter = m_viewports.begin(); iter != m_viewports.end(); ++iter)
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
void RenderTarget::setEnable(bool enable)
{
  m_enabled = enable;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END