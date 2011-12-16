#include "Core/Graphics/IndexBuffer.h"
#include "Core/Data/DataBuffer.h"
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IndexBuffer::IndexBuffer(Application* app, u32 uid) : Object(app, uid), 
                                                      m_size(EGEIndexBuffer::IS_UNKNOWN), 
                                                      m_locked(false)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IndexBuffer::~IndexBuffer()
{
  destroy();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates buffer for requested number of indicies of given size. */
bool IndexBuffer::create(EGEIndexBuffer::IndexSize size, u32 count)
{
  EGE_UNUSED(count);

  EGE_ASSERT(!m_locked);

  // store index size
  m_size = size;
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Destroys buffer. */
void IndexBuffer::destroy()
{
  // clear data
  m_locked = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns index size (in bytes). */
u8 IndexBuffer::indexSize() const
{
  switch (m_size)
  {
    case EGEIndexBuffer::IS_8BIT:  return 1; 
    case EGEIndexBuffer::IS_16BIT: return 2; 
    case EGEIndexBuffer::IS_32BIT: return 4; 
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
