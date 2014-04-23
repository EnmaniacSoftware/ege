#include "Core/Graphics/OpenGL/IndexBufferVBOOGL.h"
#include "EGEDataBuffer.h"
#include "EGEOpenGL.h"
#include "EGEDebug.h"
#include "EGEMath.h"
#include "EGEDevice.h"

EGE_NAMESPACE_BEGIN

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
      
    default:
      break;
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

    default:
      break;
  }

  return GL_WRITE_ONLY;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IndexBufferVBO::IndexBufferVBO(Application* app, const String& name, EGEIndexBuffer::UsageType usage) : IndexBuffer(app, name),
                                                                                                        m_id(0),
                                                                                                        m_indexCount(0),
                                                                                                        m_indexCapacity(0),
                                                                                                        m_lockOffset(0),
                                                                                                        m_lockLength(0),
                                                                                                        m_mapping(NULL),
                                                                                                        m_usage(usage)
{
  // allocate shadow buffer if no mapping is supported
  if ( ! Device::HasRenderCapability(EGEDevice::RENDER_CAPS_MAP_BUFFER))
  {
    m_shadowBuffer = ege_new DataBuffer();
  }

  // generate OGL buffer
  glGenBuffers(1, &m_id);
  OGL_CHECK();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IndexBufferVBO::~IndexBufferVBO()
{
  if (0 != m_id)
  {
    glDeleteBuffers(1, &m_id);
    OGL_CHECK();
    m_id = 0;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool IndexBufferVBO::isValid() const
{
  // check if no shadow buffer if mapping is not available
  if ( ! Device::HasRenderCapability(EGEDevice::RENDER_CAPS_MAP_BUFFER) && (NULL == m_shadowBuffer))
  {
    // error!
    return false;
  }

  return (0 < m_id);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool IndexBufferVBO::setSize(u32 count)
{
  EGE_ASSERT(!m_locked);
  EGE_ASSERT(EGEIndexBuffer::IS_UNKNOWN != m_indexSize);

  // allocate buffer
  if ( ! reallocateBuffer(count))
  { 
    // error!
    return false;
  }

  // set index count
  m_indexCount = count;

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* IndexBufferVBO::lock(u32 offset, u32 count)
{
  EGE_ASSERT(!m_locked);

  void* buffer = NULL;

  // check if and any data to lock
  if (0 < count)
  {
    // check if inside the buffer
    if ((offset + count) <= m_indexCapacity)
    {
      // check if shadow buffer should be used
      if (NULL != m_shadowBuffer)
      {
        // return pointer to requested offset
        buffer = m_shadowBuffer->data(static_cast<s64>(offset) * indexSize());
      }
      else
      {
	      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
        OGL_CHECK();

        // check if content is discardable
	      if (m_usage & EGEIndexBuffer::UT_DISCARDABLE)
	      {
	  	    // discard the buffer
	  	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount() * indexSize(), NULL, MapUsageType(m_usage));
          OGL_CHECK();

          // update engine info
          //ENGINE_INFO(VBOBufferDataCalls++);
        }

        // map the buffer
	      buffer = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, MapUsageTypeToAccessType(m_usage));
        OGL_CHECK();

        // store begining of the mapping for further use
        m_mapping = buffer;

        if (NULL != buffer)            
        {            
	        // return offsetted
	        buffer = static_cast<void*>(static_cast<u8*>(buffer) + offset * indexSize());
        }
        else
        {
          egeWarning(KOpenGLDebugName) << "Mapping failed!";
        }
      }

      if (buffer)
      {
        m_lockOffset = offset;
        m_lockLength = count;

        // store data
        m_locked = true;
      }
    }
  }

  return buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void IndexBufferVBO::unlock(void* data)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
  OGL_CHECK();

  // check if locked on shadow buffer
  if (NULL != m_shadowBuffer)
  {
    // sanity (in range) check 
    if (NULL != data)
    {
      EGE_ASSERT(reinterpret_cast<u8*>(data) < reinterpret_cast<u8*>(m_shadowBuffer->data()) + indexCount() * indexSize());
    }

    // check if content is discardable
	  if (m_usage & EGEIndexBuffer::UT_DISCARDABLE)
	  {
	  	// discard the buffer
	  	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount() * indexSize(), NULL, MapUsageType(m_usage));
      OGL_CHECK();

      // update engine info
      //ENGINE_INFO(VBOBufferDataCalls++);
    }

    // check if entire buffer was locked
    if ((0 == m_lockOffset) && (m_lockLength == indexCount()))
    {
      // update buffer at once
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<u32>(m_shadowBuffer->size()), m_shadowBuffer->data(), MapUsageType(m_usage));
      OGL_CHECK();

      // update engine info
     // ENGINE_INFO(VBOBufferDataCalls++);
    }
    else
    {
      glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_lockOffset * indexSize(), m_lockLength * indexSize(), 
                      m_shadowBuffer->data(static_cast<s64>(m_lockOffset) * indexSize()));
      OGL_CHECK();

      // update engine info
      //ENGINE_INFO(VBOBufferSubDataCalls++);
    }
  }
  else
  {
    // sanity (in range) check 
    if (NULL != data)
    {
      EGE_ASSERT(reinterpret_cast<u8*>(data) < reinterpret_cast<u8*>(m_mapping) + indexCount() * indexSize());
    }

    // unmap
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    OGL_CHECK();

    // clean up
    m_mapping = NULL;
  }

  // store data
  m_locked = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool IndexBufferVBO::reallocateBuffer(u32 count)
{
  // check if requested count is NOT within capacity
  if (count > indexCapacity())
  {
    // bind buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    OGL_CHECK();

    // allocate enough space
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * indexSize(), NULL, MapUsageType(m_usage));
    OGL_CHECK();

    // update engine info
    //ENGINE_INFO(VBOBufferDataCalls++);

    // allocate enough space in shadow buffer if required
    if (NULL != m_shadowBuffer)
    {
      // allocate shadow buffer
      if (EGE_SUCCESS != m_shadowBuffer->setSize(static_cast<s64>(count) * indexSize()))
      {
        // error!
        return false;
      }
    }

    // change capacity
    m_indexCapacity = count;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void IndexBufferVBO::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
  OGL_CHECK();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void IndexBufferVBO::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  OGL_CHECK();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* IndexBufferVBO::offset() const
{
  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
GLuint IndexBufferVBO::id() const
{
  return m_id;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END