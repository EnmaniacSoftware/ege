#include "Core/Graphics/OpenGL/IndexBufferVBOOGL.h"
#include "Core/Data/DataBuffer.h"
#include <EGEDebug.h>
#include <EGEDevice.h>

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
IndexBufferVBO::IndexBufferVBO(Application* app, EGEIndexBuffer::UsageType usage) : IndexBuffer(app, EGE_OBJECT_UID_INDEX_BUFFER_VBO, usage),
                                                                                    m_id(0),
                                                                                    m_indexCount(0),
                                                                                    m_indexCapacity(0),
                                                                                    m_shadowBufferLock(false),
                                                                                    m_lockOffset(0),
                                                                                    m_lockLength(0),
                                                                                    m_mapping(NULL)
{
  glGenBuffers(1, &m_id);
  if (GL_NO_ERROR != glGetError())
  {
    // error!
    EGE_PRINT("ERROR: Could not generate buffer!");
    m_id = 0;
  }

  // allocate shadow buffer
  m_shadowBuffer = ege_new DataBuffer();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IndexBufferVBO::~IndexBufferVBO()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! VertexBuffer override. Returns TRUE if object is valid. */
bool IndexBufferVBO::isValid() const
{
  return (0 < m_id) && (NULL != m_shadowBuffer);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IndexBuffer override. Sets buffer to given size. 
 * @param count Number of indicies buffer should contain.
 * @return Returns TRUE if success. Otherwise, FALSE.
 */
bool IndexBufferVBO::setSize(u32 count)
{
  EGE_ASSERT(!m_locked);
  EGE_ASSERT(EGEIndexBuffer::IS_UNKNOWN != m_indexSize);

  // allocate buffer
  if (!reallocateBuffer(count))
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
  EGE_ASSERT(!m_locked);

  void* buffer = NULL;

  // make sure buffer is big enough
  if (!reallocateBuffer(offset + count))
  {
    // error!
    return NULL;
  }

  // update vertex count
  m_indexCount = Math::Max(m_indexCount, offset + count);

  // check if scratch buffer should be used
  // TAGE - some threshold should be here ie 32K ?
  if (true || !Device::HasRenderCapability(EGEDevice::RENDER_CAPS_MAP_BUFFER))
  {
    // set flags to indicate lock done on shadow buffer
    m_shadowBufferLock = true;      

    // return pointer to requested offset
    buffer = m_shadowBuffer->data(static_cast<s64>(offset) * indexSize());
  }
  else
  {
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

      // store map pointer to begining of requested data for further use
      m_mapping = buffer;
    }
    else
    {
      EGE_PRINT("ERROR: Mapping failed!");
    }
  }

  if (buffer)
  {
    m_lockOffset = offset;
    m_lockLength = count;

    // store data
    m_locked = true;
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
    EGE_PRINT("ERROR: Could not bind buffer.");
    return;
  }

  // check if locked on shadow buffer
  if (m_shadowBufferLock)
  {
    // check if entire buffer was locked
    if ((0 == m_lockOffset) && (m_lockLength == indexCount()))
    {
      // update buffer at once
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<u32>(m_shadowBuffer->size()), m_shadowBuffer->data(), MapUsageType(m_usage));
      if (GL_NO_ERROR != glGetError())
      {
        EGE_PRINT("ERROR: Could not update entire buffer.");
      }
    }
    else
    {
      // check if content is discardable
	    if (m_usage & EGEIndexBuffer::UT_DISCARDABLE)
	    {
	  	  // discard the buffer
	  	  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount() * indexSize(), NULL, MapUsageType(m_usage));
	    }

      glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_lockOffset * indexSize(), m_lockLength * indexSize(), 
                      m_shadowBuffer->data(static_cast<s64>(m_lockOffset) * indexSize()));
      if (GL_NO_ERROR != glGetError())
      {
        // error!
        EGE_PRINT("ERROR: Could not update buffer.");
      }
    }

    // reset flag
    m_shadowBufferLock = false;
  }
  else
  {
    // update shadow buffer first
    EGE_MEMCPY(m_shadowBuffer->data(static_cast<s64>(m_lockOffset) * indexSize()), m_mapping, m_lockOffset * indexSize());

    // unmap
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    if (GL_NO_ERROR != glGetError())
    {
      // error!
      EGE_PRINT("ERROR: Could not unmap buffer.");
    }

    // reset data
    m_mapping = NULL;
  }

  // store data
  m_locked = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Reallocates internal buffer to accomodate given number of vertices. */
bool IndexBufferVBO::reallocateBuffer(u32 count)
{
  // check if requested count is NOT within capacity
  if (count > indexCapacity())
  {
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

    // allocate shadow buffer
    if (EGE_SUCCESS != m_shadowBuffer->setSize(static_cast<s64>(count) * indexSize()))
    {
      // error!
      return false;
    }

    // change capacity
    m_indexCapacity = count;
  }

  return true;
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
