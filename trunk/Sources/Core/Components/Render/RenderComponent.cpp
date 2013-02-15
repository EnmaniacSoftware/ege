#include "Core/Application/Application.h"
#include "Core/Graphics/IndexBuffer.h"
#include "Core/Graphics/VertexBuffer.h"
#include "Core/Graphics/Material.h"
#include "Core/Graphics/Render/RenderSystem.h"
#include "Core/Components/Component.h"
#include "Core/Components/Render/RenderComponent.h"
#include "Core/Crypto/Hash.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(RenderComponent)
EGE_DEFINE_DELETE_OPERATORS(RenderComponent)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderComponent::RenderComponent(Application* app, const String& name, s32 priority, EGEGraphics::RenderPrimitiveType primitive, 
                                 EGEVertexBuffer::UsageType vertexUsage, EGEIndexBuffer::UsageType indexUsage) 
: IComponent(app, EGE_OBJECT_UID_RENDER_COMPONENT, name), 
  m_priority(priority), 
  m_primitiveType(primitive), 
  m_hash(0), 
  m_hashInvalid(true), 
  m_clipRect(Rectf::INVALID),
  m_pointSize(1.0f),
  m_lineWidth(1.0f)
{
  m_indexBuffer  = app->graphics()->hardwareResourceProvider()->createIndexBuffer(indexUsage);
  m_vertexBuffer = app->graphics()->hardwareResourceProvider()->createVertexBuffer(vertexUsage);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderComponent::~RenderComponent()
{
  app()->graphics()->hardwareResourceProvider()->destroyIndexBuffer(m_indexBuffer);
  app()->graphics()->hardwareResourceProvider()->destroyVertexBuffer(m_vertexBuffer);

  m_indexBuffer  = NULL;
  m_vertexBuffer = NULL;
  m_material     = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderComponent::setMaterial(const PMaterial& material) 
{ 
  if (m_material != material)
  {
    m_material = material;

    invalidateHash();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool RenderComponent::isValid() const
{
  return (NULL != m_indexBuffer) && m_indexBuffer->isValid() && (NULL != m_vertexBuffer) && m_vertexBuffer->isValid();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderComponent::setPriority(s32 priority)
{
  if (m_priority != priority)
  {
    m_priority = priority;

    // emit
    emit renderPriorityChanged(this);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderComponent::invalidateHash()
{
  m_hashInvalid = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderComponent::validateHash()
{
  m_hashInvalid = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 RenderComponent::hash()
{
  if (isHashInvalid())
  {
    calculateHash();
    validateHash();
  }

  return m_hash;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderComponent::calculateHash()
{
  u32 hash = 0;

  // determine hash component from semantics
  // NOTE: generated value is from range [0 - 100000) - first 16 bits
  // NOTE: max occurences of a particular array is 9
  //const DynamicArray<EGEVertexBuffer::SARRAYSEMANTIC>& semantics = vertexBuffer()->semantics();
  //for (DynamicArray<EGEVertexBuffer::SARRAYSEMANTIC>::const_iterator it = semantics.begin(); it != semantics.end(); ++it)
  //{
  //  switch (it->type)
  //  {
  //    case EGEVertexBuffer::ARRAY_TYPE_POSITION_XYZ:  hash += 1; break;
  //    case EGEVertexBuffer::ARRAY_TYPE_COLOR_RGBA:    hash += 10; break;
  //    case EGEVertexBuffer::ARRAY_TYPE_NORMAL:        hash += 100; break;
  //    case EGEVertexBuffer::ARRAY_TYPE_TEXTURE_UV:    hash += 1000; break;
  //    case EGEVertexBuffer::ARRAY_TYPE_TANGENT:       hash += 10000; break;
  //  }
  //}

  //// determine hash component from index buffer - bit no 17
  //if (indexBuffer())
  //{
  //  hash |= (1 << 17);
  //}

  // determine hash component from textures
  if (material())
  {
    /* Hash format is 32-bit, divided as follows (high to low bits)
        bits   purpose
        4     Pass index (i.e. max 16 passes!)
        14     Hashed texture name from unit 0
        14     Hashed texture name from unit 1

        Note that at the moment we don't sort on the 3rd texture unit plus
        on the assumption that these are less frequently used; sorting on
        the first 2 gives us the most benefit for now.
    */

    PObject texture1;// = material()->texture(0);
    PObject texture2;// = material()->texture(1);

    hash = (material()->passCount() << 28) | ((reinterpret_cast<u32>(texture2.m_object) & 0x00003fff) << 14) | 
           (reinterpret_cast<u32>(texture1.m_object) & 0x00003fff);
  }
  
  // store new hash
  m_hash = hash;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderComponent::setClipRect(const Rectf& rect)
{
  m_clipRect = rect;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderComponent::setPointSize(float32 size)
{
  EGE_ASSERT(0 <= size);
  m_pointSize = size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderComponent::setLineWidth(float32 width)
{
  EGE_ASSERT(0 <= width);
  m_lineWidth = width;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END
