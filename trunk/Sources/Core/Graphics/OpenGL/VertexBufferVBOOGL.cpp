#include "Core/Graphics/OpenGL/VertexBufferVBOOGL.h"
#include "Core/Data/DataBuffer.h"
#include "Core/Debug/EngineInfo.h"
#include "EGEOpenGL.h"
#include "EGEMath.h"
#include "EGEDevice.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(VertexBufferVBO)
EGE_DEFINE_DELETE_OPERATORS(VertexBufferVBO)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Maps vertex buffer usage type to OpenGL compilant one. */
static GLenum MapUsageType(NVertexBuffer::UsageType type)
{
  switch (type)
  {
    case NVertexBuffer::UT_STATIC_WRITE:  return GL_STATIC_DRAW;
    case NVertexBuffer::UT_DYNAMIC_WRITE: return GL_DYNAMIC_DRAW;
      
    default:
      break;
  }

  return GL_DYNAMIC_DRAW;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Maps vertex buffer usage type appriopriate OpenGL compilant access type. */
static GLenum MapUsageTypeToAccessType(NVertexBuffer::UsageType type)
{
  switch (type)
  {
    case NVertexBuffer::UT_STATIC_WRITE:
    case NVertexBuffer::UT_DYNAMIC_WRITE: 
      
      return GL_WRITE_ONLY;
      
    default:
      break;
  }

  return GL_WRITE_ONLY;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VertexBufferVBO::VertexBufferVBO(Application* app, NVertexBuffer::UsageType usage) : VertexBuffer(app, EGE_OBJECT_UID_VERTEX_BUFFER_VBO),
                                                                                       m_id(0),
                                                                                       m_vertexCount(0),
                                                                                       m_vertexCapacity(0),
                                                                                       m_lockOffset(0),
                                                                                       m_lockLength(0),
                                                                                       m_mapping(NULL)
{
  // set usage
  m_usage = usage;

  // allocate shadow buffer if no mapping is supported
  if ( ! Device::HasRenderCapability(EGEDevice::RENDER_CAPS_MAP_BUFFER))
  {
    m_shadowBuffer = ege_new DataBuffer();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VertexBufferVBO::~VertexBufferVBO()
{
  // NOTE: at this point object should be deallocated
  EGE_ASSERT(0 == m_id);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool VertexBufferVBO::isValid() const
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
bool VertexBufferVBO::setSize(u32 count)
{
  EGE_ASSERT(!m_locked);

  // check if there is NO buffer to be created
  if (vertexDeclaration().vertexElements().empty())
  {
    // error!
    return false;
  }

  // allocate buffer
  if ( ! reallocateBuffer(count))
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

  // check if and any data to lock
  if (0 < count)
  {
    // check if inside the buffer
    if ((offset + count) <= m_vertexCapacity)
    {
      // check if shadow buffer should be used
      if (NULL != m_shadowBuffer)
      {
        // return pointer to requested offset
        buffer = m_shadowBuffer->data(static_cast<s64>(offset) * vertexDeclaration().vertexSize());
      }
      else
      {
	      glBindBuffer(GL_ARRAY_BUFFER, m_id);
        OGL_CHECK();

        // check if content is discardable
	      if (m_usage & NVertexBuffer::UT_DISCARDABLE)
	      {
	  	    // discard the buffer
	  	    glBufferData(GL_ARRAY_BUFFER, vertexCount() * vertexDeclaration().vertexSize(), NULL, MapUsageType(m_usage));
          OGL_CHECK();

          // update engine info
          ENGINE_INFO(m_VBOBufferDataCalls++);
        }

        // map the buffer
	      buffer = glMapBuffer(GL_ARRAY_BUFFER, MapUsageTypeToAccessType(m_usage));
        OGL_CHECK();

        // store begining of the mapping for further use
        m_mapping = buffer;

        if (NULL != buffer)            
        {                  
	        // return offsetted
	        buffer = static_cast<void*>(static_cast<u8*>(buffer) + offset * vertexDeclaration().vertexSize());
        }
        else
        {
          egeWarning(KOpenGLDebugName) << "Mapping failed!";
        }
      }

      if (NULL != buffer)
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
void VertexBufferVBO::unlock(const void* data)
{
  // check if nothing locked
  if ( ! m_locked)
  {
    // do nothing
    return;
  }

	glBindBuffer(GL_ARRAY_BUFFER, m_id);
  OGL_CHECK();

  // check if locked on shadow buffer
  if (NULL != m_shadowBuffer)
  {
    // sanity (in range) check 
    if (NULL != data)
    {
      EGE_ASSERT(reinterpret_cast<const u8*>(data) < reinterpret_cast<u8*>(m_shadowBuffer->data()) + vertexCount() * vertexDeclaration().vertexSize());
    }

    // check if content is discardable
	  if (m_usage & NVertexBuffer::UT_DISCARDABLE)
	  {
	  	// discard the buffer
	  	glBufferData(GL_ARRAY_BUFFER, vertexCount() * vertexDeclaration().vertexSize(), NULL, MapUsageType(m_usage));
      OGL_CHECK();

      // update engine info
      ENGINE_INFO(m_VBOBufferDataCalls++);
	  }

    // check if entire buffer was locked
    if ((0 == m_lockOffset) && (m_lockLength == vertexCount()))
    {
      // update buffer at once
      glBufferData(GL_ARRAY_BUFFER, static_cast<u32>(m_shadowBuffer->size()), m_shadowBuffer->data(), MapUsageType(m_usage));
      OGL_CHECK();

      // update engine info
      ENGINE_INFO(m_VBOBufferDataCalls++);
    }
    else
    {
      glBufferSubData(GL_ARRAY_BUFFER, m_lockOffset * vertexDeclaration().vertexSize(), m_lockLength * vertexDeclaration().vertexSize(), 
                      m_shadowBuffer->data(static_cast<s64>(m_lockOffset) * vertexDeclaration().vertexSize()));
      OGL_CHECK();

      // update engine info
      ENGINE_INFO(m_VBOBufferSubDataCalls++);
    }
  }
  else
  {
    // sanity (in range) check 
    if (NULL != data)
    {
      EGE_ASSERT(reinterpret_cast<const u8*>(data) < reinterpret_cast<u8*>(m_mapping) + vertexCount() * vertexDeclaration().vertexSize());
    }

    // unmap
    glUnmapBuffer(GL_ARRAY_BUFFER);
    OGL_CHECK();

    // clean up
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
    glBufferData(GL_ARRAY_BUFFER, count * vertexDeclaration().vertexSize(), NULL, MapUsageType(m_usage));
    OGL_CHECK();

    // update engine info
    ENGINE_INFO(m_VBOBufferDataCalls++);

    // allocate enough space in shadow buffer if required
    if (NULL != m_shadowBuffer)
    {
      // allocate shadow buffer
      if (EGE_SUCCESS != m_shadowBuffer->setSize(static_cast<s64>(count) * vertexDeclaration().vertexSize()))
      {
        // error!
        return false;
      }
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
  // call base class
  VertexBuffer::destroy();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END