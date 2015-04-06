#ifndef EGE_CORE_GRAPHICS_RENDER_RENDERQUEUE_H
#define EGE_CORE_GRAPHICS_RENDER_RENDERQUEUE_H

/*! @brief  This class represents base class for more specialized render queues. */

#include "EGE.h"
#include "EGEGraphics.h"
#include "EGERenderComponent.h"
#include "EGERenderer.h"
#include "EGEMatrix.h"
#include "EGEList.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IComponentRenderer;
EGE_PREDECLARE_SMART_CLASS(RenderQueue, PRenderQueue)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RenderQueue : public Object
                  , public IRenderer
{
  public:

    RenderQueue(u32 uid, u32 priority, EGEGraphics::RenderPrimitiveType primitiveType);
    virtual ~RenderQueue();

  public:

    /*! Checks if given component is suitable for insertion for a given render queue type.
     *  @param  uid       Unique id of the render queue.
     *  @param  component Component to test.
     *  @return TRUE if component is suitable for insertion.
     */
    static bool IsSuitable(u32 uid, const PRenderComponent& component);

  public:

    /*! Clears (empties) queue. */
    virtual void clear() = 0;

    /*! Returns render priority. */
    u32 priority() const;
    /*! Sets render priority. 
     *  @param  priority  Render priority.
     */
    void setPriority(u32 priority);
    
    /*! Renders queue. */
    void render(IComponentRenderer& renderer);

    /*! Returns render primitve type. */
    EGEGraphics::RenderPrimitiveType primitiveType() const;

  protected:

    /*! Render data structure. */
    struct SRENDERDATA
    {
      PRenderComponent component;   /*< Render component. */
      Matrix4f modelMatrix;         /*< Model transformation matrix. */
    };

    typedef List<SRENDERDATA> RenderDataList;

  private:

    /*! Prepares render list for rendering. 
     *  @param  list  List to fill in with render data.
     */
    virtual void prepareRenderList(RenderDataList& list) = 0;

  private:

    /*! Render priority. */
    u32 m_priority;
    /*! Render primitive. */
    EGEGraphics::RenderPrimitiveType m_primitiveType;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_RENDER_RENDERQUEUE_H