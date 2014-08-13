#include "Core/Graphics/Render/Implementation/SimpleRenderQueue.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(SimpleRenderQueue)
EGE_DEFINE_DELETE_OPERATORS(SimpleRenderQueue)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SimpleRenderQueue::SimpleRenderQueue(u32 priority, EGEGraphics::RenderPrimitiveType primitiveType) 
: RenderQueue(EGE_OBJECT_UID_SIMPLE_RENDER_QUEUE, priority, primitiveType)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SimpleRenderQueue::~SimpleRenderQueue()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SimpleRenderQueue::addForRendering(const PRenderComponent& component, const Matrix4f& modelMatrix)
{
  SRENDERDATA data;

  data.modelMatrix  = modelMatrix;
  data.component    = component;

  m_renderData.push_back(data);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SimpleRenderQueue::addForRendering(const PRenderQueue& queue)
{
  EGE_UNUSED(queue);
  return EGE_ERROR_NOT_SUPPORTED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SimpleRenderQueue::clear()
{
  m_renderData.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SimpleRenderQueue::prepareRenderList(RenderDataList& list)
{
  list = m_renderData;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END