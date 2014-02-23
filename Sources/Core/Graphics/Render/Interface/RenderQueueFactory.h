#ifndef EGE_CORE_GRAPHICS_RENDER_RENDERQUEUEFACTORY_H
#define EGE_CORE_GRAPHICS_RENDER_RENDERQUEUEFACTORY_H

/*! @brief  This class is a factory for render queues. */

#include "EGE.h"
#include "EGEGraphics.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Application;
EGE_DECLARE_SMART_CLASS(RenderQueue, PRenderQueue)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RenderQueueFactory
{
  public:

    /*! Creates render queue of a given id.
     *  @param  app           Pointer to application.
     *  @param  uid           Unique id of the render queue to be created.
     *  @param  priority      Render priority of the queue.
     *  @param  primitiveType Type of render primitive.
     *  @return Created object on success. Otherwise, NULL.
     */
    static PRenderQueue Create(Application* app, u32 uid, u32 priority, EGEGraphics::RenderPrimitiveType primitiveType);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_RENDER_RENDERQUEUEFACTORY_H