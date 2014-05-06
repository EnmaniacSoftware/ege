#include "Core/Graphics/Render/Implementation/SimpleRenderQueue.h"
#include "Core/Graphics/Render/Implementation/ComponentRenderer.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(SimpleRenderQueue)
EGE_DEFINE_DELETE_OPERATORS(SimpleRenderQueue)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SimpleRenderQueue::SimpleRenderQueue(Application* app, u32 priority, EGEGraphics::RenderPrimitiveType primitiveType) 
: RenderQueue(app, EGE_OBJECT_UID_SIMPLE_RENDER_QUEUE, priority, primitiveType)
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

  m_renderData.insert(component->hash(), data);

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
void SimpleRenderQueue::render(IComponentRenderer& renderer)
{
  for (RenderDataMap::const_iterator it = m_renderData.begin(); it != m_renderData.end(); ++it)
  {
    const SRENDERDATA& data = it->second;

    renderer.renderComponent(data.component, data.modelMatrix);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END