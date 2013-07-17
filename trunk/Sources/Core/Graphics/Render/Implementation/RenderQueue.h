#ifndef EGE_CORE_GRAPHICS_RENDER_RENDERQUEUE_H
#define EGE_CORE_GRAPHICS_RENDER_RENDERQUEUE_H

/*! @brief  This class represents base class for more specialized render queues. */

#include "EGE.h"
#include "Core/Math/Matrix4.h"
#include "Core/Components/Render/RenderComponent.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Application;
class IComponentRenderer;
EGE_DECLARE_SMART_CLASS(RenderQueue, PRenderQueue)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RenderQueue : public Object
{
  public:

    RenderQueue(Application* app) : Object(app) {}
    virtual ~RenderQueue() {}

  public:

    /*! Adds given data for rendering. */
    virtual bool addForRendering(const PRenderComponent& component, const Matrix4f& worldMatrix = Matrix4f::IDENTITY) = 0;
    /*! Clears (empties) queue. */
    virtual void clear() = 0;
    /*! Renders queue. */
    virtual void render(IComponentRenderer& renderer) = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_RENDER_RENDERQUEUE_H