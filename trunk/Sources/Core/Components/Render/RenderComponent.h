#ifndef EGE_CORE_RENDERCOMPONENT_H
#define EGE_CORE_RENDERCOMPONENT_H

#include <EGE.h>
#include <EGEGraphics.h>
#include <EGEVertexBuffer.h>
#include <EGESignal.h>
#include "Core/Components/Component.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(RenderComponent, PRenderComponent)
EGE_DECLARE_SMART_CLASS(Material, PMaterial)
EGE_DECLARE_SMART_CLASS(VertexBuffer, PVertexBuffer)
EGE_DECLARE_SMART_CLASS(IndexBuffer, PIndexBuffer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RenderComponent : public IComponent
{
  public:

    RenderComponent(Application* app, const String& name, s32 priority = EGEGraphics::RP_MAIN, 
                    EGEGraphics::RenderPrimitiveType primitive = EGEGraphics::RPT_TRIANGLES, 
                    EGEVertexBuffer::UsageType vertexUsage = EGEVertexBuffer::UT_DYNAMIC_WRITE,
                    EGEIndexBuffer::UsageType indexUsage = EGEIndexBuffer::UT_DYNAMIC_WRITE);
   ~RenderComponent();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  signals:

    /*! Signal emitted when render priority changes.
        @param  Sender Sender object.
     */
    Signal1<PRenderComponent> renderPriorityChanged;

  public:

    /* IComponent override. Returns TRUE if component is valid. */
    bool isValid() const override;
    /*! Returns index buffer. */
    inline PIndexBuffer indexBuffer() const { return m_indexBuffer; }
    /*! Returns vertex buffer. */  
    inline PVertexBuffer vertexBuffer() const { return m_vertexBuffer; }
    /* Sets render priority. */
    void setPriority(s32 priority);
    /*! Returns render priority. */
    inline s32 priority() const { return m_priority; }
    /*! Returns render primitive type. */
    inline EGEGraphics::RenderPrimitiveType primitiveType() const { return m_primitiveType; }
    /*! Returns render material. */
    inline PMaterial material() const { return m_material; }
    /* Sets render material. */
    void setMaterial(const PMaterial& material);
    /* Returns hash. */
    u32 hash();
    /* Sets clipping rectangle. */
    void setClipRect(const Rectf& rect);
    /*! Returns clipping rectangle. */
    inline const Rectf& clipRect() const { return m_clipRect; }
    /* Sets point size. */
    void setPointSize(float32 size);
    /*! Returns point size. */
    inline float32 pointSize() const { return m_pointSize; }
    /* Sets line width. */
    void setLineWidth(float32 width);
    /*! Returns line width. */
    inline float32 lineWidth() const { return m_lineWidth; }

  private:

    /* Invalidates hash value. */
    void invalidateHash();
    /* Validates hash value. */
    void validateHash();
    /*! Returns TRUE if hash value needs to be recalulated. */
    inline bool isHashInvalid() const { return m_hashInvalid; }
    /* Calculates hash. */
    void calculateHash();

  private:

    /*! Index buffer. */
    PIndexBuffer m_indexBuffer;
    /*! Vertex buffer. */
    PVertexBuffer m_vertexBuffer;
    /*! Render priority. */
    s32 m_priority;
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