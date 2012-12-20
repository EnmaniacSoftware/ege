#include "Core/Graphics/OpenGL/VertexBufferVBOOGL.h"
#include "Core/Data/DataBuffer.h"
#include <EGEOpenGL.h>
#include <EGEDebug.h>
#include <EGEDevice.h>

EGE_NAMESPACE_BEGIN

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
VertexBufferVBO::VertexBufferVBO(Application* app, EGEVertexBuffer::UsageType usage) : VertexBuffer(app, EGE_OBJECT_UID_VERTEX_BUFFER_VBO, usage),
                                                                                       m_id(0),
                                                                                       m_vertexCount(0),
                                                                                       m_vertexCapacity(0),
                                                                                       m_shadowBufferLock(false),
                                                                                       m_lockOffset(0),
                                                                                       m_lockLength(0),
                                                                                       m_mapping(NULL)
{
  glGenBuffers(1, &m_id);
  OGL_CHECK();

  // allocate shadow buffer
  m_shadowBuffer = ege_new DataBuffer();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VertexBufferVBO::~VertexBufferVBO()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool VertexBufferVBO::isValid() const
{
  return (0 < m_id) && (NULL != m_shadowBuffer);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool VertexBufferVBO::setSize(u32 count)
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

  // update amount of data used
  m_vertexCount = count;

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* VertexBufferVBO::lock(u32 offset, u32 count)
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
  m_vertexCount = Math::Max(m_vertexCount, offset + count);

  // check if scratch buffer should be used
  // TAGE - some threshold should be here ie 32K ?
  if (true || !Device::HasRenderCapability(EGEDevice::RENDER_CAPS_MAP_BUFFER))
  {
    // set flags to indicate lock done on shadow buffer
    m_shadowBufferLock = true;      

    // return pointer to requested offset
    buffer = m_shadowBuffer->data(static_cast<s64>(offset) * vertexSize());
  }
  else
  {
	  glBindBuffer(GL_ARRAY_BUFFER, m_id);
    OGL_CHECK();

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

      // store map pointer to begining of requested data for further use
      m_mapping = buffer;
    }
    else
    {
      egeWarning() << "Mapping failed!";
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
void VertexBufferVBO::unlock(void* data)
{
  EGE_UNUSED(data);

	glBindBuffer(GL_ARRAY_BUFFER, m_id);
  OGL_CHECK();

  // check if locked on shadow buffer
  if (m_shadowBufferLock)
  {
    // check if entire buffer was locked
    if ((0 == m_lockOffset) && (m_lockLength == vertexCount()))
    {
      // update buffer at once
      glBufferData(GL_ARRAY_BUFFER, static_cast<u32>(m_shadowBuffer->size()), m_shadowBuffer->data(), MapUsageType(m_usage));
      OGL_CHECK();
    }
    else
    {
      // check if content is discardable
	    if (m_usage & EGEVertexBuffer::UT_DISCARDABLE)
	    {
	  	  // discard the buffer
	  	  glBufferData(GL_ARRAY_BUFFER, vertexCount() * vertexSize(), NULL, MapUsageType(m_usage));
	    }

      glBufferSubData(GL_ARRAY_BUFFER, m_lockOffset * vertexSize(), m_lockLength * vertexSize(), 
                      m_shadowBuffer->data(static_cast<s64>(m_lockOffset) * vertexSize()));
      OGL_CHECK();
    }

    // reset flag
    m_shadowBufferLock = false;
  }
  else
  {
    // update shadow buffer first
    EGE_MEMCPY(m_shadowBuffer->data(static_cast<s64>(m_lockOffset) * vertexSize()), m_mapping, m_lockOffset * vertexSize());

    // unmap
    glUnmapBuffer(GL_ARRAY_BUFFER);
    OGL_CHECK();

    // reset data
    m_mapping = NULL;
  }

  // store data
  m_locked = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool VertexBufferVBO::reallocateBuffer(u32 count)
{
  // check if requested count is NOT within capacity
  if (count > vertexCapacity())
  {
    // bind buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    OGL_CHECK();

    // allocate enough space
    glBufferData(GL_ARRAY_BUFFER, count * vertexSize(), NULL, MapUsageType(m_usage));
    OGL_CHECK();

    // allocate shadow buffer
    if (EGE_SUCCESS != m_shadowBuffer->setSize(static_cast<s64>(count) * vertexSize()))
    {
      // error!
      return false;
    }

    // change capacity
    m_vertexCapacity = count;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void VertexBufferVBO::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
  OGL_CHECK();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void VertexBufferVBO::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
  OGL_CHECK();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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

EGE_NAMESPACE_END