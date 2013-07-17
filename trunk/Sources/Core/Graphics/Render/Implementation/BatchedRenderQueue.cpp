#include "Core/Graphics/Render/Implementation/BatchedRenderQueue.h"
  
EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(BatchedRenderQueue)
EGE_DEFINE_DELETE_OPERATORS(BatchedRenderQueue)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BatchedRenderQueue::BatchedRenderQueue(Application* app) : RenderQueue(app)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BatchedRenderQueue::~BatchedRenderQueue()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool BatchedRenderQueue::addForRendering(const PRenderComponent& component, const Matrix4f& worldMatrix)
{
  EGE_ASSERT((EGEGraphics::RPT_TRIANGLES == component->primitiveType()) || (EGEGraphics::RPT_TRIANGLE_STRIPS == component->primitiveType()));

  // calculate hash
  u32 hash = component->hash();

  SRENDERDATA data;

  data.worldMatrix  = worldMatrix;
  data.component    = component;

  m_renderData.insert(component->hash(), data);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void BatchedRenderQueue::clear()
{
  m_renderData.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void BatchedRenderQueue::render(IComponentRenderer& renderer)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END