#include "Core/Graphics/OpenGL/VertexBufferVAOGL.h"
#include "Core/Data/DataBuffer.h"
#include <EGEList.h>
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(VertexBufferVA)
EGE_DEFINE_DELETE_OPERATORS(VertexBufferVA)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VertexBufferVA::VertexBufferVA(Application* app) : VertexBuffer(app, EGE_OBJECT_UID_VERTEX_BUFFER_VA)
{
  m_buffer = ege_new DataBuffer();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VertexBufferVA::~VertexBufferVA()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool VertexBufferVA::isValid() const
{
  return (NULL != m_buffer);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool VertexBufferVA::setSize(u32 count)
{
  EGE_ASSERT(!m_locked);

  // check if there is NO buffer to be created
  if (m_semantics.empty())
  {
    // error!
    return false;
  }

  // allocate buffer
  if (!reallocateBuffer(count))
  {
    // error!
    return false;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void VertexBufferVA::destroy()
{
  m_buffer = NULL;

  // call base class
  VertexBuffer::destroy();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* VertexBufferVA::lock(u32 offset, u32 count)
{
  EGE_ASSERT(!m_locked);

  // check if and any data to lock
  if (0 <= count)
  {
    // check if NOT enough space in buffer
    if (offset + count > vertexCapacity())
    {
      // reallocate buffer
      if (!reallocateBuffer(offset + count))
      {
        // error!
        return NULL;
      }
    }

    // set lock flag
    m_locked = true;

    // update amount of data used
    m_buffer->setSize((offset + count) * vertexSize());

    // return offsetted
    return reinterpret_cast<void*>(reinterpret_cast<u8*>(m_buffer->data()) + offset * vertexSize());
  }
  
  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void VertexBufferVA::unlock(void* data)
{
  if (data)
  {
    EGE_ASSERT(reinterpret_cast<u8*>(data) < reinterpret_cast<u8*>(m_buffer->data()) + vertexCapacity() * vertexSize());
  }

  m_locked = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 VertexBufferVA::vertexCount() const
{
  return (m_buffer) ? static_cast<u32>(m_buffer->size() / vertexSize()) : 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool VertexBufferVA::reallocateBuffer(u32 count)
{
  // allocate buffer for requested indicies
  if (EGE_SUCCESS != m_buffer->setSize(vertexSize() * count))
  {
    // error!
    return false;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 VertexBufferVA::vertexCapacity() const
{
  return (m_buffer) ? static_cast<u32>(m_buffer->capacity() / vertexSize()) : 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END