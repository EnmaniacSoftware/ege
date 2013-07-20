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
EGEResult BatchedRenderQueue::addForRendering(const PRenderComponent& component, const Matrix4f& modelMatrix)
{
  EGEResult result = EGE_SUCCESS;

  // check if not allocated yet
  if (NULL == m_renderData)
  {
    // allocate
    if ( ! allocateMasterRenderComponent(component))
    {
      // failed
      result = EGE_ERROR;
    }
  }
  else
  {
    // verify if component is compatible
    // NOTE: Materials must be the same:
    //       - textures
    //       - shaders
    //       - blend operators
    //
    //       Vertex semantics must be the same.
    //if (component->vertexBuffer()->semantics() 
  }

  if (EGE_SUCCESS == result)
  {
    // append component
    result = appendComponent(component) ? EGE_SUCCESS : EGE_ERROR;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void BatchedRenderQueue::clear()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void BatchedRenderQueue::render(IComponentRenderer& renderer)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool BatchedRenderQueue::allocateMasterRenderComponent(const PRenderComponent& component)
{
  bool result = false;

  // allocate master component
  m_renderData = ege_new RenderComponent(app(), String::Format("BatchedRenderQueue@%d", component->priority()), component->vertexBuffer()->vertexDeclaration(), 
                                         component->priority(), component->primitiveType(), NVertexBuffer::UT_DYNAMIC_WRITE);
  if (NULL != m_renderData)
  {
    // setup vertex buffer semantics
    if (m_renderData->isValid())
    {
      // error!
      return NULL;
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool BatchedRenderQueue::appendComponent(const PRenderComponent& component)
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END