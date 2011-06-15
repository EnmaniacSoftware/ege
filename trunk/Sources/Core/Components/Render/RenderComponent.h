#ifndef EGE_CORE_RENDERCOMPONENT_H
#define EGE_CORE_RENDERCOMPONENT_H

#include <EGE.h>
#include <EGEGraphics.h>
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

    RenderComponent(Application* app, const EGEString& name, s32 priority = EGEGraphics::RENDER_PRIORITY_MAIN,
                    EGEGraphics::ERenderPrimitiveType primitive = EGEGraphics::RENDER_PRIMITIVE_TYPE_TRIANGLES);
    virtual ~RenderComponent();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* IComponent override. Returns TRUE if component is valid. */
    virtual bool isValid() const override;
    /*! Returns index buffer. */
    inline PIndexBuffer indexBuffer() const { return m_indexBuffer; }
    /*! Returns vertex buffer. */  
    inline PVertexBuffer vertexBuffer() const { return m_vertexBuffer; }
    /*! Returns render priority. */
    inline s32 priority() const { return m_priority; }
    /*! Returns render primitive type. */
    inline EGEGraphics::ERenderPrimitiveType primitiveType() const { return m_primitiveType; }
    /*! Returns render material. */
    inline PMaterial material() const { return m_material; }
    /* Sets render material. */
    void setMaterial(const PMaterial& material);

  private:

    /*! Index buffer. */
    PIndexBuffer m_indexBuffer;
    /*! Vertex buffer. */
    PVertexBuffer m_vertexBuffer;
    /*! Render priority. */
    s32 m_priority;
    /*! Render primitive type. */
    EGEGraphics::ERenderPrimitiveType m_primitiveType;
    /*! Render material. */
    PMaterial m_material;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RENDERCOMPONENT_H