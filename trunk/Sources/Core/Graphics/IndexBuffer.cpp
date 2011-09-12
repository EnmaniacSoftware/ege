#include "Core/Graphics/IndexBuffer.h"
#include "Core/Data/DataBuffer.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(IndexBuffer)
EGE_DEFINE_DELETE_OPERATORS(IndexBuffer)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IndexBuffer::IndexBuffer(Application* app) : Object(app), m_size(SIZE_UNKNOWN), m_locked(false)
{
  m_buffer = ege_new DataBuffer();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IndexBuffer::~IndexBuffer()
{
  destroy();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool IndexBuffer::isValid() const
{
  return NULL != m_buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates buffer for requested number of indicies of given size. */
bool IndexBuffer::create(Size size, u32 count)
{
  // store indicies size
  m_size = size;

  // allocate buffer
  if (!reallocateBuffer(count))
  {
    // error!
    return false;
  }

  m_locked = false;

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Destroys buffer. */
void IndexBuffer::destroy()
{
  m_buffer = NULL;

  // clear data
  m_locked = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Locks buffer given part of the buffer for read/write operations. */
void* IndexBuffer::lock(u32 offset, u32 count)
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
/*! Unlocks buffer. */
void IndexBuffer::unlock()
{
  m_locked = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns number of allocated indicies. */
u32 IndexBuffer::indexCount() const
{
  return (m_buffer && (0 < indexSize())) ? static_cast<u32>(m_buffer->size() / indexSize()) : 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns index size (in bytes). */
u8 IndexBuffer::indexSize() const
{
  switch (m_size)
  {
    case SIZE_8BIT:  return 1; 
    case SIZE_16BIT: return 2; 
    case SIZE_32BIT: return 4; 
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Reallocates internal buffer to accomodate given number of indicies. */
bool IndexBuffer::reallocateBuffer(u32 count)
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
