#include "Core/Graphics/Render/Interface/RenderQueue.h"
#include "Core/Graphics/Render/Implementation/BatchedRenderQueue.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderQueue::RenderQueue(Application* app, u32 uid, u32 priority, EGEGraphics::RenderPrimitiveType primitiveType) : Object(app, uid),
                                                                                                                    m_priority(priority),
                                                                                                                    m_primitiveType(primitiveType)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderQueue::~RenderQueue()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool RenderQueue::IsSuitable(u32 uid, const PRenderComponent& component)
{
  bool result = false;

  switch (uid)
  {
    case EGE_OBJECT_UID_BACTHED_RENDER_QUEUE: 
    
      result = BatchedRenderQueue::IsSuitable(component); 
      break;

    case EGE_OBJECT_UID_SIMPLE_RENDER_QUEUE:

      result = true;
      break;

    default:

      EGE_ASSERT_X(false, "Unknown render queue ID!");
      break;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 RenderQueue::priority() const
{
  return m_priority;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEGraphics::RenderPrimitiveType RenderQueue::primitiveType() const
{
  return m_primitiveType;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END
