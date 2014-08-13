#ifndef EGE_CORE_GRAPHICS_RENDER_SIMPLERENDERQUEUE_H
#define EGE_CORE_GRAPHICS_RENDER_SIMPLERENDERQUEUE_H

#include "EGE.h"
#include "EGERenderComponent.h"
#include "EGEMatrix.h"
#include "Core/Graphics/Render/Interface/RenderQueue.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SimpleRenderQueue : public RenderQueue
{
  public:

    SimpleRenderQueue(u32 priority, EGEGraphics::RenderPrimitiveType primitiveType);
    virtual ~SimpleRenderQueue();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  private:

    /*! @see RenderQueue::addForRendering. */
    EGEResult addForRendering(const PRenderComponent& component, const Matrix4f& modelMatrix = Matrix4f::IDENTITY) override;
    /*! @see RenderQueue::addForRendering. */
    EGEResult addForRendering(const PRenderQueue& queue) override;
    /*! @see RenderQueue::clear. */
    void clear() override;
    /*! @see RenderQueue::prepareRenderList. */
    void prepareRenderList(RenderDataList& list) override;

  private:

    /*! Render data. */
    RenderDataList m_renderData;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_RENDER_SIMPLERENDERQUEUE_H