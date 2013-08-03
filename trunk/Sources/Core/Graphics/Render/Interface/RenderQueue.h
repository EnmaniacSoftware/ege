#ifndef EGE_CORE_GRAPHICS_RENDER_RENDERQUEUE_H
#define EGE_CORE_GRAPHICS_RENDER_RENDERQUEUE_H

/*! @brief  This class represents base class for more specialized render queues. */

#include "EGE.h"
#include "EGEGraphics.h"
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

    RenderQueue(Application* app, u32 uid, u32 priority, EGEGraphics::RenderPrimitiveType primitiveType);
    virtual ~RenderQueue();

  public:

    /*! Checks if given component is suitable for insertion for a given render queue type.
     *  @param  uid       Unique id of the render queue.
     *  @param  component Component to test.
     *  @return TRUE if component is suitable for insertion.
     */
    static bool IsSuitable(u32 uid, const PRenderComponent& component);

  public:

    /*! Adds given data for rendering. 
     *  @param  component   Component to add for later rendering.
     *  @param  modelMatrix Model transformation matrix.
     *  @return EGE_SUCCESS if given component has been added. EGE_ERROR_NOT_SUPPORTED if component has been rejected by render queue. EGE_ERROR otherwise.
     *  @note Render queue may reject component due to invalid semantics or material. If this happens new render queue of similar kind can be created to accomodate
     *        component.
     */
    virtual EGEResult addForRendering(const PRenderComponent& component, const Matrix4f& modelMatrix = Matrix4f::IDENTITY) = 0;
    /*! Clears (empties) queue. */
    virtual void clear() = 0;
    /*! Renders queue. */
    virtual void render(IComponentRenderer& renderer) = 0;
    /*! Returns render priority. */
    u32 priority() const;
    /*! Returns render primitve type. */
    EGEGraphics::RenderPrimitiveType primitiveType() const;

  private:

    /*! Render priority. */
    u32 m_priority;
    /*! Render primitive. */
    EGEGraphics::RenderPrimitiveType m_primitiveType;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_RENDER_RENDERQUEUE_H