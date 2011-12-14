#include "Core/Graphics/OpenGL/HardwareVertexBufferOGL.h"
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(HardwareVertexBufferPrivate)
EGE_DEFINE_DELETE_OPERATORS(HardwareVertexBufferPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Maps vertex buffer usage type to OpenGL compilant one. */
static GLenum MapUsageType(EGEVertexBuffer::UsageType type)
{
  switch (type)
  {
    case EGEVertexBuffer::UT_STATIC_WRITE:  return GL_STATIC_DRAW;
    case EGEVertexBuffer::UT_DYNAMIC_WRITE: return GL_DYNAMIC_DRAW;
  };

  return GL_DYNAMIC_DRAW;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Maps vertex buffer usage type appriopriate OpenGL compilant access type. */
static GLenum MapUsageTypeToAccessType(EGEVertexBuffer::UsageType type)
{
  switch (type)
  {
    case EGEVertexBuffer::UT_STATIC_WRITE:
    case EGEVertexBuffer::UT_DYNAMIC_WRITE: 
      
      return GL_WRITE_ONLY;
  };

  return GL_WRITE_ONLY;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
HardwareVertexBufferPrivate::HardwareVertexBufferPrivate(HardwareVertexBuffer* base) : m_d(base),
                                                                                       m_id(0)
{
  glGenBuffers(1, &m_id);
  if (GL_NO_ERROR != glGetError())
  {
    // error!
    m_id = 0;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
HardwareVertexBufferPrivate::~HardwareVertexBufferPrivate()
{
  if (0 < m_id)
  {
    glDeleteBuffers(1, &m_id);
    m_id = 0;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool HardwareVertexBufferPrivate::isValid() const
{
  return (0 < m_id);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates buffer for requested number of vertices. */
bool HardwareVertexBufferPrivate::create(u32 count)
{
  // bind buffer
  glBindBuffer(GL_ARRAY_BUFFER, m_id);
  if (GL_NO_ERROR != glGetError())
  {
    // error!
    return false;
  }

  // allocate enough space
  glBufferData(GL_ARRAY_BUFFER, count * d_func()->vertexSize(), NULL, MapUsageType(d_func()->m_usage));
  if (GL_NO_ERROR != glGetError())
  {
    // error!
    return false;
  }

  // set vertex count
  d_func()->m_vertexCount = count;

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Locks buffer's given part of the buffer for read/write operations. 
 * @param offset  0-based vertex offset from which locking should be done. 
 * @param count   Number of vertices to lock.
 */
void* HardwareVertexBufferPrivate::lock(u32 offset, u32 count)
{
  void* buffer = NULL;

  // check if NOT locked yet and any data to lock
  if (!d_func()->m_locked && (0 <= count))
  {
    // check if NOT enough space in buffer
    if (offset + count > d_func()->vertexCount())
    {
      // reallocate buffer
      if (!reallocateBuffer(offset + count))
      {
        // error!
        return NULL;
      }
    }

	  glBindBuffer(GL_ARRAY_BUFFER, m_id);
    if (GL_NO_ERROR != glGetError())
    {
      // error!
      return NULL;
    }

    // check if content is discardable
	  if (d_func()->m_usage & EGEVertexBuffer::UT_DISCARDABLE)
	  {
	  	// discard the buffer
	  	glBufferData(GL_ARRAY_BUFFER, d_func()->vertexCount() * d_func()->vertexSize(), NULL, MapUsageType(d_func()->m_usage));
	  }

    // map the buffer
	  buffer = glMapBuffer(GL_ARRAY_BUFFER, MapUsageTypeToAccessType(d_func()->m_usage));
    if (buffer)            
    {            
	    // return offsetted
	    buffer = static_cast<void*>(static_cast<u8*>(buffer) + offset);

      // store data
      d_func()->m_locked = true;
    }
  }

  return buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Unlocks buffer. */
void HardwareVertexBufferPrivate::unlock()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
  if (GL_NO_ERROR != glGetError())
  {
    // error!
    EGE_PRINT("HardwareVertexBufferPrivate::unlock - Could not bind buffer.");
    return;
  }

  glUnmapBuffer(GL_ARRAY_BUFFER);
  if (GL_NO_ERROR != glGetError())
  {
    // error!
    EGE_PRINT("HardwareVertexBufferPrivate::unlock - Could not unmap buffer.");
  }

  // store data
  d_func()->m_locked = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Reallocates internal buffer to accomodate given number of vertices. */
bool HardwareVertexBufferPrivate::reallocateBuffer(u32 count)
{
  return create(count);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Binds buffer. */
bool HardwareVertexBufferPrivate::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
  return (GL_NO_ERROR == glGetError());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Unbinds buffer. */
bool HardwareVertexBufferPrivate::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
  return (GL_NO_ERROR == glGetError());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
