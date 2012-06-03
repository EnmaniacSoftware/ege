#include "Core/Graphics/Render/RenderQueue.h"
  
EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(RenderQueue)
EGE_DEFINE_DELETE_OPERATORS(RenderQueue)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderQueue::RenderQueue(Application* app) : Object(app)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderQueue::~RenderQueue()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds given data for rendering. */
bool RenderQueue::addForRendering(const PRenderComponent& component, const Matrix4f& worldMatrix)
{
  SRENDERDATA data;

  data.worldMatrix  = worldMatrix;
  data.component    = component;

  m_renderData.insert(component->hash(), data);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Clears (empties) queue. */
void RenderQueue::clear()
{
  m_renderData.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns map of render data structures. */
const MultiMap<u32, RenderQueue::SRENDERDATA>& RenderQueue::renderData() const
{
  return m_renderData;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END