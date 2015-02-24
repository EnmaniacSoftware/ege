#include "Core/Graphics/IndexBuffer.h"
#include "Core/Graphics/Material.h"
#include "Core/Graphics/Render/RenderSystem.h"
#include "Core/Component/Interface/Component.h"
#include "Core/Graphics/Render/Interface/RenderComponent.h"
#include "EGEEngine.h"
#include "EGEHash.h"
#include "EGEVertexBuffer.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(RenderComponent)
EGE_DEFINE_DELETE_OPERATORS(RenderComponent)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderComponent::RenderComponent(Engine& engine, const String& name, const VertexDeclaration& vertexDeclaration, s32 priority, 
                                 EGEGraphics::RenderPrimitiveType primitive, NVertexBuffer::UsageType vertexUsage, EGEIndexBuffer::UsageType indexUsage) 
: Component(EGE_OBJECT_UID_RENDER_COMPONENT, name)
, m_engine(engine)
, m_priority(priority) 
, m_primitiveType(primitive)
, m_hash(0)
, m_hashInvalid(true)
, m_clipRect(Rectf::INVALID)
, m_pointSize(1.0f)
, m_lineWidth(1.0f)
{
  // register self in render system
  // NOTE: unregistration is done automatically
  PRenderComponent me(*this);
  engine.graphics()->hardwareResourceProvider()->registerComponent(me, vertexUsage, vertexDeclaration, indexUsage);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderComponent::~RenderComponent()
{
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

    // invalidate VAOs
    removeAllVertexArrayObjects();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool RenderComponent::isValid() const
{
  return (NULL != m_indexBuffer) && m_indexBuffer->isValid() && (NULL != m_vertexBuffer) && m_vertexBuffer->isValid();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderComponent::setPriority(u32 priority)
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
  // NOTE: generated value is from range [0 - 100000)
  // NOTE: max occurences of a particular array is 9
  const VertexElementArray& vertexElementArray = vertexBuffer()->vertexDeclaration().vertexElements();
  for (VertexElementArray::const_iterator it = vertexElementArray.begin(); it != vertexElementArray.end(); ++it)
  {
    switch (it->semantic())
    {
      case NVertexBuffer::VES_POSITION_XYZ:  hash += 1; break;
      case NVertexBuffer::VES_COLOR_RGBA:    hash += 10; break;
      case NVertexBuffer::VES_NORMAL:        hash += 100; break;
      case NVertexBuffer::VES_TEXTURE_UV:    hash += 1000; break;
      case NVertexBuffer::VES_TANGENT:       hash += 10000; break;
      
      default:
     
        // TAGE - use all possible values ?
        break;
    }
  }

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

    //PObject texture1;// = material()->texture(0);
    //PObject texture2;// = material()->texture(1);

  //  hash = (material()->passCount() << 28) | ((reinterpret_cast<u32>(texture2.m_object) & 0x00003fff) << 14) | 
       //    (reinterpret_cast<u32>(texture1.m_object) & 0x00003fff);
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
EGEResult RenderComponent::addComponent(const PComponent& component)
{
  EGEResult result = EGE_SUCCESS;

  // add apprioprietly
  switch (component->uid())
  {
    case EGE_OBJECT_UID_VERTEX_BUFFER: 
      
      // NOTE: pointer check
      if (m_vertexBuffer != component)
      {
        m_vertexBuffer = component;

        // invalidate VAOs
        removeAllVertexArrayObjects();
      }
      break;
    
    case EGE_OBJECT_UID_INDEX_BUFFER: 
      
      // NOTE: pointer check
      if (m_indexBuffer != component)
      {
        m_indexBuffer = component; 

        // invalidate VAOs
        removeAllVertexArrayObjects();
      }
      break;

    default:

      result = Component::addComponent(component);
      break;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderComponent::onProgramChanged(PRenderPass pass)
{
  // remove all vertex array objects
  removeAllVertexArrayObjects();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderComponent::removeAllVertexArrayObjects()
{
  List<PComponent> list = components(EGE_OBJECT_UID_VERTEX_ARRAY_OBJECT);
  for (List<PComponent>::ConstIterator it = list.begin(); it != list.end(); ++it)
  {
    removeComponent(*it);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Engine& RenderComponent::engine() const
{
  return m_engine;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END
