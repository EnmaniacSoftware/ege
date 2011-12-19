#include "Core/Graphics/IndexBuffer.h"
#include "Core/Data/DataBuffer.h"
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IndexBuffer::IndexBuffer(Application* app, u32 uid, EGEIndexBuffer::UsageType usage) : Object(app, uid), 
                                                                                       m_indexSize(EGEIndexBuffer::IS_UNKNOWN), 
                                                                                       m_locked(false),
                                                                                       m_usage(usage)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IndexBuffer::~IndexBuffer()
{
  destroy();
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
  switch (m_indexSize)
  {
    case EGEIndexBuffer::IS_8BIT:  return 1; 
    case EGEIndexBuffer::IS_16BIT: return 2; 
    case EGEIndexBuffer::IS_32BIT: return 4; 
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets index size. */
void IndexBuffer::setIndexSize(EGEIndexBuffer::IndexSize size)
{
  m_indexSize = size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
