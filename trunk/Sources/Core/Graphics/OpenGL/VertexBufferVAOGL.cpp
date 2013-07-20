#include "Core/Graphics/OpenGL/VertexBufferVAOGL.h"
#include "Core/Data/DataBuffer.h"
#include "EGEList.h"
#include "EGEDebug.h"

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
  if (vertexDeclaration().vertexElements().empty())
  {
    // error!
    return false;
  }

  // allocate buffer
  if ( ! reallocateBuffer(count))
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
  if (0 < count)
  {
    // check if inside the buffer
    if ((offset + count) * vertexDeclaration().vertexSize() <= m_buffer->size())
    {
      // set lock flag
      m_locked = true;

      // return begining of the block
      return reinterpret_cast<void*>(reinterpret_cast<u8*>(m_buffer->data()) + offset * vertexDeclaration().vertexSize());
    }
  }

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void VertexBufferVA::unlock(void* data)
{
  if (data)
  {
    EGE_ASSERT(reinterpret_cast<u8*>(data) < reinterpret_cast<u8*>(m_buffer->data()) + vertexCount() * vertexDeclaration().vertexSize());
  }

  m_locked = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 VertexBufferVA::vertexCount() const
{
  return (m_buffer) ? static_cast<u32>(m_buffer->size() / vertexDeclaration().vertexSize()) : 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool VertexBufferVA::reallocateBuffer(u32 count)
{
  // allocate buffer for requested indicies
  if (EGE_SUCCESS != m_buffer->setSize(vertexDeclaration().vertexSize() * count))
  {
    // error!
    return false;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END