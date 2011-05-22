#ifndef EGE_CORE_RENDERCOMPONENT_H
#define EGE_CORE_RENDERCOMPONENT_H

#include "EGE.h"
#include "Core/Components/Component.h"
#include "Core/Graphics/IndexBuffer.h"
#include "Core/Graphics/VertexBuffer.h"
#include "Core/Graphics/Material.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(RenderComponent, PRenderComponent)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class RenderComponent : public IComponent
{
  public:

    RenderComponent(Application* app, const EGEString& name);
    virtual ~RenderComponent();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    enum RenderPriority
    {
      RENDER_PRIORITY_BACKGROUND       = 0,
      RENDER_PRIORITY_MAIN             = 50,
//      RENDER_PRIORITY_MAIN_PRE_OVERLAY = 95,
      RENDER_PRIORITY_MAIN_OVERLAY     = 99,
      RENDER_PRIORITY_MAIN_CURSOR      = 100
    };

    enum PrimitiveType
    {
      PRIMITIVE_TYPE_TRIANGLES = 0,
      PRIMITIVE_TYPE_LINES
    };

    /* IComponent override. Returns TRUE if component is valid. */
    virtual bool isValid() const override;
    /*! Returns index buffer. */
    inline PIndexBuffer indexBuffer() const { return m_indexBuffer; }
    /*! Returns vertex buffer. */  
    inline PVertexBuffer vertexBuffer() const { return m_vertexBuffer; }
    /* Sets render priority. */
    void setRenderPriority(RenderPriority priority);
    /*! Returns render priority. */
    inline RenderPriority renderPriority() const { return m_renderPriority; }
    /* Sets render primitive type. */
    void setPrimitiveType(PrimitiveType type);
    /*! Returns render primitive type. */
    inline PrimitiveType primitiveType() const { return m_primitiveType; }
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
    RenderPriority m_renderPriority;
    /*! Render primitive type. */
    PrimitiveType m_primitiveType;
    /*! Render material. */
    PMaterial m_material;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RENDERCOMPONENT_H