#include "Core/Graphics/IndexBuffer.h"
#include "Core/Data/DataBuffer.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IndexBuffer::IndexBuffer(Application* app, u32 uid) : Component(app, uid, ""), 
                                                      m_indexSize(EGEIndexBuffer::IS_UNKNOWN), 
                                                      m_locked(false),
                                                      m_usage(EGEIndexBuffer::UT_STATIC_WRITE)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
IndexBuffer::~IndexBuffer()
{
  destroy();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void IndexBuffer::destroy()
{
  // clear data
  m_locked = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u8 IndexBuffer::indexSize() const
{
  switch (m_indexSize)
  {
    case EGEIndexBuffer::IS_8BIT:  return 1; 
    case EGEIndexBuffer::IS_16BIT: return 2; 
    case EGEIndexBuffer::IS_32BIT: return 4;
      
    default:
      
      return 0;
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void IndexBuffer::setIndexSize(EGEIndexBuffer::IndexSize size)
{
  m_indexSize = size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END