#include "Core/Graphics/OpenGL/IndexBufferVBOOGL.h"
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(IndexBufferVBO)
EGE_DEFINE_DELETE_OPERATORS(IndexBufferVBO)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Maps index buffer usage type to OpenGL compilant one. */
static GLenum MapUsageType(EGEIndexBuffer::UsageType type)
{
  switch (type)
  {
    case EGEIndexBuffer::UT_STATIC_WRITE:  return GL_STATIC_DRAW;
    case EGEIndexBuffer::UT_DYNAMIC_WRITE: return GL_DYNAMIC_DRAW;
  }

  return GL_DYNAMIC_DRAW;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Maps index buffer usage type appriopriate OpenGL compilant access type. */
static GLenum MapUsageTypeToAccessType(EGEIndexBuffer::UsageType type)
{
  switch (type)
  {
    case EGEIndexBuffer::UT_STATIC_WRITE:
    case EGEIndexBuffer::UT_DYNAMIC_WRITE: 
      
      return GL_WRITE_ONLY;
  }

  return GL_WRITE_ONLY;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IndexBufferVBO::IndexBufferVBO(Application* app) : IndexBuffer(app, EGE_OBJECT_UID_INDEX_BUFFER_VBO),
                                                   m_id(0),
                                                   m_indexCount(0)
{
  // TAGE
  m_usage = EGEIndexBuffer::UT_DYNAMIC_WRITE;

  glGenBuffers(1, &m_id);
  if (GL_NO_ERROR != glGetError())
  {
    // error!
    m_id = 0;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IndexBufferVBO::~IndexBufferVBO()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! VertexBuffer override. Returns TRUE if object is valid. */
bool IndexBufferVBO::isValid() const
{
  return (0 < m_id);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! VertexBuffer override. Creates buffer for requested number of vertices. */
bool IndexBufferVBO::create(EGEIndexBuffer::IndexSize size, u32 count)
{
  // call base class
  if (!IndexBuffer::create(size, count))
  {
    // error!
    return false;
  }

  // bind buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
  if (GL_NO_ERROR != glGetError())
  {
    // error!
    return false;
  }

  // allocate enough space
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * indexSize(), NULL, MapUsageType(m_usage));
  if (GL_NO_ERROR != glGetError())
  {
    // error!
    return false;
  }

  // set index count
  m_indexCount = count;

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Locks buffer's given part of the buffer for read/write operations. 
 * @param offset  0-based vertex offset from which locking should be done. 
 * @param count   Number of vertices to lock.
 */
void* IndexBufferVBO::lock(u32 offset, u32 count)
{
  void* buffer = NULL;

  // check if NOT locked yet and any data to lock
  if (!m_locked && (0 <= count))
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

	  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    if (GL_NO_ERROR != glGetError())
    {
      // error!
      return NULL;
    }

    // check if content is discardable
	  if (m_usage & EGEIndexBuffer::UT_DISCARDABLE)
	  {
	  	// discard the buffer
	  	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount() * indexSize(), NULL, MapUsageType(m_usage));
	  }

    // map the buffer
	  buffer = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, MapUsageTypeToAccessType(m_usage));
    if (buffer)            
    {            
	    // return offsetted
	    buffer = static_cast<void*>(static_cast<u8*>(buffer) + offset);

      // store data
      m_locked = true;
    }
  }

  return buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Unlocks buffer. */
void IndexBufferVBO::unlock()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
  if (GL_NO_ERROR != glGetError())
  {
    // error!
    EGE_PRINT("IndexBufferVBO::unlock - Could not bind buffer.");
    return;
  }

  glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
  if (GL_NO_ERROR != glGetError())
  {
    // error!
    EGE_PRINT("IndexBufferVBO::unlock - Could not unmap buffer.");
  }

  // store data
  m_locked = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Reallocates internal buffer to accomodate given number of vertices. */
bool IndexBufferVBO::reallocateBuffer(u32 count)
{
  return create(m_size, count);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Binds buffer. */
bool IndexBufferVBO::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
  return (GL_NO_ERROR == glGetError());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Unbinds buffer. */
bool IndexBufferVBO::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  return (GL_NO_ERROR == glGetError());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! VertexBuffer override. Destroys buffer. */
void IndexBufferVBO::destroy()
{
  if (0 < m_id)
  {
    glDeleteBuffers(1, &m_id);
    m_id = 0;
  }

  // call base class
  IndexBuffer::destroy();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
