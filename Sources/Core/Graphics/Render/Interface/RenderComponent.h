#ifndef EGE_CORE_RENDERCOMPONENT_H
#define EGE_CORE_RENDERCOMPONENT_H

#include "EGE.h"
#include "EGEGraphics.h"
#include "EGEVertexBuffer.h"
#include "EGESignal.h"
#include "EGEComponent.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(RenderComponent, PRenderComponent)
EGE_DECLARE_SMART_CLASS(Material, PMaterial)
EGE_DECLARE_SMART_CLASS(VertexBuffer, PVertexBuffer)
EGE_DECLARE_SMART_CLASS(IndexBuffer, PIndexBuffer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RenderComponent : public Component
{
  public:

    RenderComponent(Engine& engine, const String& name, const VertexDeclaration& vertexDeclaration, s32 priority = EGEGraphics::RP_MAIN, 
                    EGEGraphics::RenderPrimitiveType primitive = EGEGraphics::RPT_TRIANGLES,
                    NVertexBuffer::UsageType vertexUsage = NVertexBuffer::UT_DYNAMIC_WRITE,
                    EGEIndexBuffer::UsageType indexUsage = EGEIndexBuffer::UT_DYNAMIC_WRITE);
   ~RenderComponent();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public signals:

    /*! Signal emitted when render priority changes.
     *  @param  Sender Sender object.
     */
    Signal1<PRenderComponent> renderPriorityChanged;

  public:

    /*! @see Component::isValid. */
    bool isValid() const;
    /*! @see ComponentHost::addComponent. */
    EGEResult addComponent(const PComponent& component) override;
    /*! Returns index buffer. */
    PIndexBuffer indexBuffer() const { return m_indexBuffer; }
    /*! Returns vertex buffer. */  
    PVertexBuffer vertexBuffer() const { return m_vertexBuffer; }
    /*! Sets render priority. */
    void setPriority(u32 priority);
    /*! Returns render priority. */
    u32 priority() const { return m_priority; }
    /*! Returns render primitive type. */
    EGEGraphics::RenderPrimitiveType primitiveType() const { return m_primitiveType; }
    /*! Returns render material. */
    PMaterial material() const { return m_material; }
    /*! Sets render material. */
    void setMaterial(const PMaterial& material);
    /*! Returns hash. */
    u32 hash();
    /*! Sets clipping rectangle. */
    void setClipRect(const Rectf& rect);
    /*! Returns clipping rectangle. */
    const Rectf& clipRect() const { return m_clipRect; }
    /*! Sets point size. */
    void setPointSize(float32 size);
    /*! Returns point size. */
    float32 pointSize() const { return m_pointSize; }
    /*! Sets line width. */
    void setLineWidth(float32 width);
    /*! Returns line width. */
    float32 lineWidth() const { return m_lineWidth; }
    /*! Returns engine object. */
    Engine& engine() const;

  private:

    /*! Invalidates hash value. */
    void invalidateHash();
    /*! Validates hash value. */
    void validateHash();
    /*! Returns TRUE if hash value needs to be recalulated. */
    bool isHashInvalid() const { return m_hashInvalid; }
    /*! Calculates hash. */
    void calculateHash();
    /*! Removes all vertex array objects. */
    void removeAllVertexArrayObjects();

  private slots:

    /*! Slot called when program for given render pass has changed. 
     *  @param  pass  Render pass for which program has changed.
     */
    void onProgramChanged(PRenderPass pass);

  private:

    /*! Reference to engine. */
    Engine& m_engine;
    /*! Index buffer. */
    PIndexBuffer m_indexBuffer;
    /*! Vertex buffer. */
    PVertexBuffer m_vertexBuffer;
    /*! Render priority. */
    u32 m_priority;
    /*! Render primitive type. */
    EGEGraphics::RenderPrimitiveType m_primitiveType;
    /*! Render material. */
    PMaterial m_material;
    /*! Hash value. */
    u32 m_hash;
    /*! Hash valuidity flag. */
    bool m_hashInvalid;
    /*! Viewport clipping rectangle (in window coordinates). If INVALID entire viewport will be covered. */
    Rectf m_clipRect;
    /*! Point size. */
    float32 m_pointSize;
    /*! Line width. */
    float32 m_lineWidth;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RENDERCOMPONENT_H