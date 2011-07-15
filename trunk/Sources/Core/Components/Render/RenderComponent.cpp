#include "Core/Graphics/IndexBuffer.h"
#include "Core/Graphics/VertexBuffer.h"
#include "Core/Graphics/Material.h"
#include "Core/Components/Component.h"
#include "Core/Components/Render/RenderComponent.h"
#include "Core/Crypto/Hash.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(RenderComponent)
EGE_DEFINE_DELETE_OPERATORS(RenderComponent)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderComponent::RenderComponent(Application* app, const String& name, s32 priority, EGEGraphics::ERenderPrimitiveType primitive) 
: IComponent(app, EGE_OBJECT_UID_RENDER_COMPONENT, name), m_priority(priority), m_primitiveType(primitive), m_hash(0)
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
  if (m_material != material)
  {
    m_material = material;

    invalidateHash();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IComponent override. Returns TRUE if component is valid. */
bool RenderComponent::isValid() const
{
  return (NULL != m_indexBuffer) && m_indexBuffer->isValid() && (NULL != m_vertexBuffer) && m_vertexBuffer->isValid();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets render priority. */
void RenderComponent::setPriority(s32 priority)
{
  m_priority = priority;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Invalidates hash value. */
void RenderComponent::invalidateHash()
{
  m_hash = 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns hash. */
u32 RenderComponent::hash() const
{
  if (isHashInvalid())
  {
    calculateHash();
  }

  return m_hash;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Calculates hash. */
void RenderComponent::calculateHash() const
{
  u32 hash = 0;

  // determine hash component from semantics
  // NOTE: generated value is from range [0 - 100000) - first 16 bits
  // NOTE: max occurences of a particular array is 9
  const DynamicArray<EGEVertexBuffer::SARRAYSEMANTIC>& semantics = vertexBuffer()->semantics();
  for (DynamicArray<EGEVertexBuffer::SARRAYSEMANTIC>::const_iterator it = semantics.begin(); it != semantics.end(); ++it)
  {
    switch (it->type)
    {
      case EGEVertexBuffer::ARRAY_TYPE_POSITION_XYZ:  hash += 1; break;
      case EGEVertexBuffer::ARRAY_TYPE_COLOR_RGBA:    hash += 10; break;
      case EGEVertexBuffer::ARRAY_TYPE_NORMAL:        hash += 100; break;
      case EGEVertexBuffer::ARRAY_TYPE_TEXTURE_UV:    hash += 1000; break;
      case EGEVertexBuffer::ARRAY_TYPE_TANGENT:       hash += 10000; break;
    }
  }

  // determine hash component from index buffer - bit no 17
  if (indexBuffer())
  {
    hash |= (1 << 17);
  }

  // determine hash component from textures
  

  // store new hash
  m_hash = hash;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
