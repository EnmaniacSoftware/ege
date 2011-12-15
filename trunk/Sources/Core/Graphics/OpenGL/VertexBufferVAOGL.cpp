#include "Core/Graphics/OpenGL/VertexBufferVAOGL.h"
#include "Core/Data/DataBuffer.h"
#include <EGEList.h>
#include <EGEDebug.h>

EGE_NAMESPACE

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
  destroy();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! VertexBuffer override. Returns TRUE if object is valid. */
bool VertexBufferVA::isValid() const
{
  return (NULL != m_buffer);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! VertexBuffer override. Creates buffer for requested number of vertices. */
bool VertexBufferVA::create(u32 count)
{
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

  // store data
  m_locked = false;

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! VertexBuffer override. Destroys buffer. */
void VertexBufferVA::destroy()
{
  m_buffer = NULL;

  // call base class
  VertexBuffer::destroy();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! VertexBuffer override. Locks buffer's given part of the buffer for read/write operations. 
 *  @param offset  0-based vertex offset from which locking should be done. 
 *  @param count   Number of vertices to lock.
 */
void* VertexBufferVA::lock(u32 offset, u32 count)
{
  // check if NOT locked yet and any data to lock
  if (!m_locked && (0 <= count))
  {
    // check if NOT enough space in buffer
    if (offset + count > vertexCount())
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

    return reinterpret_cast<void*>(reinterpret_cast<u8*>(m_buffer->data()) + offset * vertexSize());
  }
  
  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! VertexBuffer override. Unlocks buffer. */
void VertexBufferVA::unlock()
{
  m_locked = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! VertexBuffer override. Returns number of allocated vertices. */
u32 VertexBufferVA::vertexCount() const
{
  return (m_buffer) ? static_cast<u32>(m_buffer->size() / vertexSize()) : 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Reallocates internal buffer to accomodate given number of indicies. */
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
