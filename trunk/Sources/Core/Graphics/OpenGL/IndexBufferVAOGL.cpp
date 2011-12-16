#include "Core/Graphics/OpenGL/IndexBufferVAOGL.h"
#include "Core/Data/DataBuffer.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(IndexBufferVA)
EGE_DEFINE_DELETE_OPERATORS(IndexBufferVA)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IndexBufferVA::IndexBufferVA(Application* app) : IndexBuffer(app, EGE_OBJECT_UID_INDEX_BUFFER_VA)
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
/*! IndexBuffer override. Creates buffer for requested number of indicies of given size. */
bool IndexBufferVA::create(EGEIndexBuffer::IndexSize size, u32 count)
{
  // call base class
  if (!IndexBuffer::create(size, count))
  {
    // error!
    return false;
  }

  // allocate buffer if necessary
  if (0 < count)
  {
    if (!reallocateBuffer(count))
    {
      // error!
      return false;
    }
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
  // check if NOT locked yet and any data to lock
  if (!m_locked && (0 < count))
  {
    // check if NOT enough space in buffer
    if (offset + count > indexCount())
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

    return reinterpret_cast<u8*>(m_buffer->data()) + offset * indexSize();
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
