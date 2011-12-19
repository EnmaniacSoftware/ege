#include "Core/Graphics/OpenGL/IndexBufferVAOGL.h"
#include "Core/Data/DataBuffer.h"
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(IndexBufferVA)
EGE_DEFINE_DELETE_OPERATORS(IndexBufferVA)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IndexBufferVA::IndexBufferVA(Application* app, EGEIndexBuffer::UsageType usage) : IndexBuffer(app, EGE_OBJECT_UID_INDEX_BUFFER_VA, usage)
{
  m_buffer = ege_new DataBuffer();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IndexBufferVA::~IndexBufferVA()
{
  destroy();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool IndexBufferVA::isValid() const
{
  return (NULL != m_buffer);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IndexBuffer override. Sets buffer to given size. 
 * @param count Number of indicies buffer should contain.
 * @return Returns TRUE if success. Otherwise, FALSE.
 */
bool IndexBufferVA::setSize(u32 count)
{
  EGE_ASSERT(!m_locked);
  EGE_ASSERT(EGEIndexBuffer::IS_UNKNOWN != m_indexSize);

  // allocate buffer
  if (!reallocateBuffer(count))
  {
    // error!
    return false;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IndexBuffer override. Destroys buffer. */
void IndexBufferVA::destroy()
{
  m_buffer = NULL;

  // call base class
  IndexBuffer::destroy();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IndexBuffer override. Locks buffer given part of the buffer for read/write operations. */
void* IndexBufferVA::lock(u32 offset, u32 count)
{
  EGE_ASSERT(!m_locked);

  // check if and any data to lock
  if (0 <= count)
  {
    // check if NOT enough space in buffer
    if (offset + count > indexCapacity())
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
    m_buffer->setSize((offset + count) * indexSize());

    // return offsetted
    return reinterpret_cast<void*>(reinterpret_cast<u8*>(m_buffer->data()) + offset * indexSize());
  }

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IndexBuffer override. Unlocks buffer. */
void IndexBufferVA::unlock()
{
  m_locked = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IndexBuffer override. Returns number of allocated indicies. */
u32 IndexBufferVA::indexCount() const
{
  return (m_buffer && (0 < indexSize())) ? static_cast<u32>(m_buffer->size() / indexSize()) : 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Reallocates internal buffer to accomodate given number of indicies. */
bool IndexBufferVA::reallocateBuffer(u32 count)
{
  // allocate buffer for requested indicies
  if (EGE_SUCCESS != m_buffer->setSize(indexSize() * count))
  {
    // error!
    return false;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IndexBuffer override. Returns maximal number of available indicies. */
u32 IndexBufferVA::indexCapacity() const
{
  return (m_buffer) ? static_cast<u32>(m_buffer->capacity() / indexSize()) : 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
