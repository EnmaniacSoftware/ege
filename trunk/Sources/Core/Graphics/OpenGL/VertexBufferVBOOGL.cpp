#include "Core/Graphics/OpenGL/VertexBufferVBOOGL.h"
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(VertexBufferVBO)
EGE_DEFINE_DELETE_OPERATORS(VertexBufferVBO)
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
VertexBufferVBO::VertexBufferVBO(Application* app) : VertexBuffer(app, EGE_OBJECT_UID_VERTEX_BUFFER_VBO),
                                                     m_id(0),
                                                     m_vertexCount(0)
{
  // TAGE
  m_usage = EGEVertexBuffer::UT_DYNAMIC_WRITE;

  glGenBuffers(1, &m_id);
  if (GL_NO_ERROR != glGetError())
  {
    // error!
    m_id = 0;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VertexBufferVBO::~VertexBufferVBO()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! VertexBuffer override. Returns TRUE if object is valid. */
bool VertexBufferVBO::isValid() const
{
  return (0 < m_id);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! VertexBuffer override. Creates buffer for requested number of vertices. */
bool VertexBufferVBO::create(u32 count)
{
  // bind buffer
  glBindBuffer(GL_ARRAY_BUFFER, m_id);
  if (GL_NO_ERROR != glGetError())
  {
    // error!
    return false;
  }

  // allocate enough space
  glBufferData(GL_ARRAY_BUFFER, count * vertexSize(), NULL, MapUsageType(m_usage));
  if (GL_NO_ERROR != glGetError())
  {
    // error!
    return false;
  }

  // set vertex count
  m_vertexCount = count;

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Locks buffer's given part of the buffer for read/write operations. 
 * @param offset  0-based vertex offset from which locking should be done. 
 * @param count   Number of vertices to lock.
 */
void* VertexBufferVBO::lock(u32 offset, u32 count)
{
  void* buffer = NULL;

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

	  glBindBuffer(GL_ARRAY_BUFFER, m_id);
    if (GL_NO_ERROR != glGetError())
    {
      // error!
      return NULL;
    }

    // check if content is discardable
	  if (m_usage & EGEVertexBuffer::UT_DISCARDABLE)
	  {
	  	// discard the buffer
	  	glBufferData(GL_ARRAY_BUFFER, vertexCount() * vertexSize(), NULL, MapUsageType(m_usage));
	  }

    // map the buffer
	  buffer = glMapBuffer(GL_ARRAY_BUFFER, MapUsageTypeToAccessType(m_usage));
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
void VertexBufferVBO::unlock()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
  if (GL_NO_ERROR != glGetError())
  {
    // error!
    EGE_PRINT("VertexBufferVBO::unlock - Could not bind buffer.");
    return;
  }

  glUnmapBuffer(GL_ARRAY_BUFFER);
  if (GL_NO_ERROR != glGetError())
  {
    // error!
    EGE_PRINT("VertexBufferVBO::unlock - Could not unmap buffer.");
  }

  // store data
  m_locked = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Reallocates internal buffer to accomodate given number of vertices. */
bool VertexBufferVBO::reallocateBuffer(u32 count)
{
  return create(count);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Binds buffer. */
bool VertexBufferVBO::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
  return (GL_NO_ERROR == glGetError());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Unbinds buffer. */
bool VertexBufferVBO::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
  return (GL_NO_ERROR == glGetError());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! VertexBuffer override. Destroys buffer. */
void VertexBufferVBO::destroy()
{
  if (0 < m_id)
  {
    glDeleteBuffers(1, &m_id);
    m_id = 0;
  }

  // call base class
  VertexBuffer::destroy();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
