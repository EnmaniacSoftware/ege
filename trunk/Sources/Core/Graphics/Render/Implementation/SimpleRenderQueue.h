#ifndef EGE_CORE_GRAPHICS_RENDER_SIMPLERENDERQUEUE_H
#define EGE_CORE_GRAPHICS_RENDER_SIMPLERENDERQUEUE_H

#include "EGE.h"
#include "EGERenderComponent.h"
#include "Core/Math/Matrix4.h"
#include "Core/Graphics/Render/Interface/RenderQueue.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Application;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SimpleRenderQueue : public RenderQueue
{
  public:

    SimpleRenderQueue(Application* app, u32 priority, EGEGraphics::RenderPrimitiveType primitiveType);
    virtual ~SimpleRenderQueue();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  private:

    /*! @see RenderQueue::addForRendering. */
    EGEResult addForRendering(const PRenderComponent& component, const Matrix4f& modelMatrix = Matrix4f::IDENTITY) override;
    /*! @see RenderQueue::clear. */
    void clear() override;
    /*! @see RenderQueue::render. */
    void render(IComponentRenderer& renderer) override;

  private:

    /*! Render data structure. */
    struct SRENDERDATA
    {
      PRenderComponent component;   /*< Render component. */
      Matrix4f modelMatrix;         /*< Model transformation matrix. */
    };

    typedef MultiMap<u32, SRENDERDATA> RenderDataMap;

    /*! Render data sorted by component hash. */
    RenderDataMap m_renderData;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_RENDER_SIMPLERENDERQUEUE_H