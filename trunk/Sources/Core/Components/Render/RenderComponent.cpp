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
RenderComponent::RenderComponent(Application* app, const EGEString& name, s32 priority, EGEGraphics::ERenderPrimitiveType primitive) 
: IComponent(app, EGE_OBJECT_UID_RENDER_COMPONENT, name), m_priority(priority), m_primitiveType(primitive)
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
/*! Sets render material. */
void RenderComponent::setMaterial(const PMaterial& material) 
{ 
  m_material = material; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IComponent override. Returns TRUE if component is valid. */
bool RenderComponent::isValid() const
{
  return (NULL != m_indexBuffer) && m_indexBuffer->isValid() && (NULL != m_vertexBuffer) && m_vertexBuffer->isValid();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
