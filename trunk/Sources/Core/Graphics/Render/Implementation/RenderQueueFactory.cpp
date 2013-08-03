#include "Core/Graphics/Render/Interface/RenderQueueFactory.h"
#include "Core/Graphics/Render/Implementation/BatchedRenderQueue.h"
#include "Core/Graphics/Render/Implementation/SimpleRenderQueue.h"
#include "Core/ObjectUIDs.h"
#include "EGEDebug.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PRenderQueue RenderQueueFactory::Create(Application* app, u32 uid, u32 priority, EGEGraphics::RenderPrimitiveType primitiveType)
{
  PRenderQueue object;

  // create according to object type
  switch (uid)
  {
    case EGE_OBJECT_UID_SIMPLE_RENDER_QUEUE:  object = ege_new SimpleRenderQueue(app, priority, primitiveType); break;
    case EGE_OBJECT_UID_BACTHED_RENDER_QUEUE: object = ege_new BatchedRenderQueue(app, priority, primitiveType); break;

    default:

      EGE_ASSERT_X(false, "Unsupported render queue type!");
      break;
  }

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------