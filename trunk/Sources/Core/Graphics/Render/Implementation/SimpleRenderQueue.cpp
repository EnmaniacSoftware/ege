#include "Core/Graphics/Render/Implementation/SimpleRenderQueue.h"
#include "Core/Graphics/Render/Implementation/ComponentRenderer.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(SimpleRenderQueue)
EGE_DEFINE_DELETE_OPERATORS(SimpleRenderQueue)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SimpleRenderQueue::SimpleRenderQueue(Application* app) : RenderQueue(app)
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