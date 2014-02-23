#include "Core/Graphics/OpenGL/IndexBufferVAOGL.h"
#include "Core/Data/DataBuffer.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(IndexBufferVA)
EGE_DEFINE_DELETE_OPERATORS(IndexBufferVA)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IndexBufferVA::IndexBufferVA(Application* app, const String& name) : IndexBuffer(app, name)
{
  m_buffer = ege_new DataBuffer();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IndexBufferVA::~IndexBufferVA()
{
  destroy();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool IndexBufferVA::isValid() const
{
  return (NULL != m_buffer);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool IndexBufferVA::setSize(u32 count)
{
  EGE_ASSERT(!m_locked);
  EGE_ASSERT(EGEIndexBuffer::IS_UNKNOWN != m_indexSize);

  // allocate buffer
  if (!reallocateBuffer(count))
  {
    // error!
    return false;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void IndexBufferVA::destroy()
{
  m_buffer = NULL;

  // call base class
  IndexBuffer::destroy();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* IndexBufferVA::lock(u32 offset, u32 count)
{
  EGE_ASSERT(!m_locked);

  // check if and any data to lock
  if (0 < count)
  {
    // check if inside the buffer
    if ((offset + count) * indexSize() <= m_buffer->size())
    {
      // set lock flag
      m_locked = true;

      // return begining of the block
      return reinterpret_cast<void*>(reinterpret_cast<u8*>(m_buffer->data()) + offset * indexSize());
    }
  }

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void IndexBufferVA::unlock(void* data)
{
  if (data)
  {
    EGE_ASSERT(reinterpret_cast<u8*>(data) < reinterpret_cast<u8*>(m_buffer->data()) + indexCount() * indexSize());
  }

  m_locked = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 IndexBufferVA::indexCount() const
{
  return (m_buffer && (0 < indexSize())) ? static_cast<u32>(m_buffer->size() / indexSize()) : 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
void IndexBufferVA::bind()
{
  // do nothing
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void IndexBufferVA::unbind()
{
  // do nothig
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* IndexBufferVA::offset() const
{
  return m_buffer->data();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END