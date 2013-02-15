#include "Core/Graphics/OpenGL/IndexBufferVBOOGL.h"
#include "Core/Data/DataBuffer.h"
#include <EGEDebug.h>
#include <EGEMath.h>
#include <EGEDevice.h>

EGE_NAMESPACE_BEGIN

#define SHADOW_BUFFER 0

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
IndexBufferVBO::IndexBufferVBO(Application* app, EGEIndexBuffer::UsageType usage) : IndexBuffer(app, EGE_OBJECT_UID_INDEX_BUFFER_VBO),
                                                                                    m_id(0),
                                                                                    m_indexCount(0),
                                                                                    m_indexCapacity(0),
                                                                                    m_shadowBufferLock(false),
                                                                                    m_lockOffset(0),
                                                                                    m_lockLength(0),
                                                                                    m_mapping(NULL)
{
  // set usage
  m_usage = usage;

  // allocate shadow buffer
  m_shadowBuffer = ege_new DataBuffer();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IndexBufferVBO::~IndexBufferVBO()
{
  // NOTE: at this point object should be deallocated
  EGE_ASSERT(0 == m_id);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool IndexBufferVBO::isValid() const
{
  return (0 < m_id) && (NULL != m_shadowBuffer);
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
  if (0 <= count)
  {
    // check if inside the buffer
    if ((offset + count) <= m_indexCapacity)
    {
  // check if scratch buffer should be used
  // TAGE - some threshold should be here ie 32K ?
#if SHADOW_BUFFER
      if (true || !Device::HasRenderCapability(EGEDevice::RENDER_CAPS_MAP_BUFFER))
      {
        // set flags to indicate lock done on shadow buffer
        m_shadowBufferLock = true;      

        // return pointer to requested offset
        buffer = m_shadowBuffer->data(static_cast<s64>(offset) * indexSize());
      }
      else
#endif
      {
	      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
        OGL_CHECK();

        // check if content is discardable
	      if (m_usage & EGEIndexBuffer::UT_DISCARDABLE)
	      {
	  	    // discard the buffer
	  	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount() * indexSize(), NULL, MapUsageType(m_usage));
          OGL_CHECK();
	      }

        // map the buffer
	      buffer = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, MapUsageTypeToAccessType(m_usage));
        OGL_CHECK();
        if (NULL != buffer)            
        {            
	        // return offsetted
	        buffer = static_cast<void*>(static_cast<u8*>(buffer) + offset * indexSize());

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
    }
  }

  return buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void IndexBufferVBO::unlock(void* data)
{
  EGE_UNUSED(data);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
  OGL_CHECK();

#if SHADOW_BUFFER
  // check if locked on shadow buffer
  if (m_shadowBufferLock)
  {
    // check if entire buffer was locked
    if ((0 == m_lockOffset) && (m_lockLength == indexCount()))
    {
      // update buffer at once
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<u32>(m_shadowBuffer->size()), m_shadowBuffer->data(), MapUsageType(m_usage));
      OGL_CHECK();
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
      OGL_CHECK();
    }

    // reset flag
    m_shadowBufferLock = false;
  }
  else
#endif
  {
#if SHADOW_BUFFER
    // update shadow buffer first
    EGE_MEMCPY(m_shadowBuffer->data(static_cast<s64>(m_lockOffset) * indexSize()), m_mapping, m_lockOffset * indexSize());
#endif 
    // unmap
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    OGL_CHECK();

    // reset data
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

#if SHADOW_BUFFER
    // allocate shadow buffer
    if (EGE_SUCCESS != m_shadowBuffer->setSize(static_cast<s64>(count) * indexSize()))
    {
      // error!
      return false;
    }
#endif

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
void IndexBufferVBO::destroy()
{
  // call base class
  IndexBuffer::destroy();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END