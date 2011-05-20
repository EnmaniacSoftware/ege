#include "Core/Graphics/IndexBuffer.h"
#include "Core/Graphics/VertexBuffer.h"
#include "Core/Graphics/Material.h"
#include "Core/Components/Component.h"
#include "Core/Components/Render/RenderComponent.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(RenderComponent)
EGE_DEFINE_DELETE_OPERATORS(RenderComponent)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderComponent::RenderComponent(Application* app, const EGEString& name) : IComponent(app, EGE_OBJECT_UID_RENDER_COMPONENT, name), 
                                                                            m_renderPriority(RENDER_PRIORITY_MAIN), m_primitiveType(PRIMITIVE_TYPE_TRIANGLES)
{
  m_indexBuffer  = ege_new IndexBuffer(app);
  m_vertexBuffer = ege_new VertexBuffer(app);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderComponent::~RenderComponent()
{
  m_indexBuffer  = NULL;
  m_vertexBuffer = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets render priority. */
void RenderComponent::setRenderPriority(RenderPriority priority)
{
  m_renderPriority = priority;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets render primitive type. */
void RenderComponent::setPrimitiveType(PrimitiveType type)
{
  m_primitiveType = type;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets render material. */
void RenderComponent::setMaterial(const PMaterial& material) 
{ 
  m_material = material; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IComponent override. Returns TRUE if component is valid. */
bool RenderComponent::isValid() const
{
  return (NULL != m_indexBuffer) && (NULL != m_vertexBuffer);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
