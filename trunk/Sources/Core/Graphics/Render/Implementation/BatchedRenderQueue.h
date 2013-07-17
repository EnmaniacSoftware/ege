#ifndef EGE_CORE_GRAPHICS_RENDER_BATCHEDRENDERQUEUE_H
#define EGE_CORE_GRAPHICS_RENDER_BATCHEDRENDERQUEUE_H

#include "EGE.h"
#include "Core/Math/Matrix4.h"
#include "Core/Graphics/Render/Implementation/RenderQueue.h"
#include "Core/Components/Render/RenderComponent.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Application;
EGE_DECLARE_SMART_CLASS(RenderQueue, PRenderQueue)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class BatchedRenderQueue : public RenderQueue
{
  public:

    BatchedRenderQueue(Application* app);
    virtual ~BatchedRenderQueue();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  private:

    /*! @see RenderQueue::addForRendering. */
    bool addForRendering(const PRenderComponent& component, const Matrix4f& worldMatrix = Matrix4f::IDENTITY) override;
    /*! @see RenderQueue::clear. */
    void clear() override;
    /*! @see RenderQueue::render. */
    void render(IComponentRenderer& renderer) override;

  public:

    /*! Render data structure. */
    struct SRENDERDATA
    {
      PRenderComponent component;   /*< Render component. */
      
      Matrix4f worldMatrix;         /*< World transformation matrix. */
    };

    typedef MultiMap<u32, SRENDERDATA> RenderDataMap;

  private:

    /*! Render data sorted by component hash. */
    RenderDataMap m_renderData;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_RENDER_BATCHEDRENDERQUEUE_H