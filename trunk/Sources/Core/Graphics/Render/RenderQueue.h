#ifndef EGE_CORE_RENDERQUEUE_H
#define EGE_CORE_RENDERQUEUE_H

#include <EGE.h>
#include "Core/Math/Matrix4.h"
#include "Core/Components/Render/RenderComponent.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Application;

EGE_DECLARE_SMART_CLASS(RenderQueue, PRenderQueue)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class RenderQueue : public Object
{
  public:

    RenderQueue(Application* app);
   ~RenderQueue();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Render data structure. */
    struct SRENDERDATA
    {
      PRenderComponent component;   /*< Render component. */
      
      Matrix4f worldMatrix;         /*< World transformation matrix. */
    };

  public:

    /*! Adds given data for rendering. */
    bool addForRendering(const Matrix4f& worldMatrix, const PRenderComponent& component);
    /* Clears (empties) queue. */
    void clear();
    /* Returns map of render data structures. */
    const MultiMap<u32, SRENDERDATA>& renderData() const;

  private:

    /*! Render data sorted by component hash. */
    MultiMap<u32, SRENDERDATA> m_renderData;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RENDERQUEUE_H