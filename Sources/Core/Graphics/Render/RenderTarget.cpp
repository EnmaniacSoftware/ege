#include "Core/Graphics/Render/RenderTarget.h"
#include "Core/Graphics/Viewport.h"
#include "Core/Graphics/Camera.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(RenderTarget)
EGE_DEFINE_DELETE_OPERATORS(RenderTarget)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderTarget::RenderTarget(const Dictionary& params) : Object()
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
  Dictionary::const_iterator iterName   = params.find(EGE_RENDER_TARGET_PARAM_NAME);
  Dictionary::const_iterator iterWidth  = params.find(EGE_RENDER_TARGET_PARAM_WIDTH);
  Dictionary::const_iterator iterHeight = params.find(EGE_RENDER_TARGET_PARAM_HEIGHT);
  Dictionary::const_iterator iterZoom   = params.find(EGE_RENDER_TARGET_PARAM_ZOOM);

  m_name   = (iterName != params.end()) ? iterName->second : "";
  m_width  = (iterWidth != params.end()) ? iterWidth->second.toInt() : 0;
  m_height = (iterHeight != params.end()) ? iterHeight->second.toInt() : 0;
  m_zoom   = (iterZoom != params.end()) ? iterZoom->second.toFloat() : 1.0f;
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
    m_viewports.push_back(viewport);
  }

  return viewport;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderTarget::removeViewport(const String& name)
{
  for (DynamicArray<PViewport>::iterator iter = m_viewports.begin(); iter != m_viewports.end(); ++iter)
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
PViewport RenderTarget::viewport(const String& name) const
{
  for (DynamicArray<PViewport>::const_iterator iter = m_viewports.begin(); iter != m_viewports.end(); ++iter)
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
  for (DynamicArray<PViewport>::iterator iter = m_viewports.begin(); iter != m_viewports.end();)
  {
    *iter = NULL;

    iter = m_viewports.erase(iter);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderTarget::render()
{
  // reset stats
  m_batchCount    = 0;
  m_vertexCount = 0;

  // go thru all viewports
  for (DynamicArray<PViewport>::const_iterator iter = m_viewports.begin(); iter != m_viewports.end(); ++iter)
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