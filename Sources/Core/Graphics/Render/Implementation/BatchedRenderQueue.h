#ifndef EGE_CORE_GRAPHICS_RENDER_BATCHEDRENDERQUEUE_H
#define EGE_CORE_GRAPHICS_RENDER_BATCHEDRENDERQUEUE_H

#include "EGE.h"
#include "EGEList.h"
#include "EGERenderComponent.h"
#include "EGEMatrix.h"
#include "Core/Graphics/Render/Interface/RenderQueue.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(RenderQueue, PRenderQueue)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class BatchedRenderQueue : public RenderQueue
{
  public:

    BatchedRenderQueue(u32 priority, EGEGraphics::RenderPrimitiveType primitiveType);
    virtual ~BatchedRenderQueue();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Checks if given component is suitable for insertion. 
     *  @param  component Component to test.
     *  @return TRUE if component is suitable for insertion.
     */
    static bool IsSuitable(const PRenderComponent& component);

  private:

    /*! @see RenderQueue::addForRendering. */
    EGEResult addForRendering(const PRenderComponent& component, const Matrix4f& modelMatrix = Matrix4f::IDENTITY) override;
    /*! @see RenderQueue::addForRendering. */
    EGEResult addForRendering(const PRenderQueue& queue) override;
    /*! @see RenderQueue::clear. */
    void clear() override;
    /*! @see RenderQueue::prepareRenderList. */
    void prepareRenderList(RenderDataList& list) override;

    /*! Allocates master render component for a given component. 
     *  @param  component Component for which master component should be used. It is used as a template.
     *  @return TRUE if master component has been allocated successfully.
     */
    bool allocateMasterRenderComponent(const PRenderComponent& component);
    /*! Appends component to master one. 
     *  @param  component   Component which should be appended to master one.
     *  @param  modelMatrix Model transformation matrix.
     *  @return TRUE if component has been sucessfully appened.
     */
    bool appendComponent(const PRenderComponent& component, const Matrix4f& modelMatrix);
    /*! Appends vertex buffer to master one.
     *  @param  buffer        Vertex buffer which should be appended to master one.
     *  @param  textureRects  List of pointers to texture rectangles.  
     *  @param  modelMatrix   Model transformation matrix.
     *  @return TRUE if component has been sucessfully appened.
     */
    bool appendBuffer(const PVertexBuffer& buffer, const List<const Rectf*>& textureRects, const Matrix4f& modelMatrix);
    /*! Appends index buffer to master one.
     *  @param  buffer  Index buffer which should be appended to master one.
     *  @return TRUE if component has been sucessfully appened.
     */
    bool appendBuffer(const PIndexBuffer& buffer);
    /*! Converts vertices.
     *  @param  outData       Buffer to write converted vertices into.
     *  @param  inData        Buffer to read vertices from for conversion.
     *  @param  count         Number of vertices to convert.
     *  @param  textureRects  List of pointers to texture rectangles for texture coords conversions.
     *  @param  modelMatrix   Model matrix used to convert vertex positions.
     */
    void convertVertices(float32* outData, const float32* inData, u32 count, const List<const Rectf*>& textureRects, const Matrix4f& modelMatrix) const;
    /*! Converts vertices.
     *  @param  outData       Buffer to write converted vertices into.
     *  @param  inData        Buffer to read vertices from for conversion.
     *  @param  count         Number of vertices to convert.
     *  @param  textureRects  List of pointers to texture rectangles for texture coords conversions.
     */
    void convertVertices(float32* outData, const float32* inData, u32 count, const List<const Rectf*>& textureRects) const;

    /*! Checks if given material is compatible with master one.
     *  @param  material  Material to check.
     *  @return TRUE if material is compatible.
     */
    bool isMaterialCompatible(const PMaterial& material) const;
    /*! Prepares master component based on given one.
     *  @param  component Reference component based on which master one should be prepared.
     */
    void prepare(const PRenderComponent& component);

  private:

    /*! Master render component. */
    PRenderComponent m_renderData;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_RENDER_BATCHEDRENDERQUEUE_H