#include "Core/Graphics/VertexBuffer.h"
#include "Core/Data/DataBuffer.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(VertexBuffer)
EGE_DEFINE_DELETE_OPERATORS(VertexBuffer)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VertexBuffer::VertexBuffer(Application* app) : Object(app), m_locked(false), m_vertexSize(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VertexBuffer::~VertexBuffer()
{
  destroy();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds given array type to overall semantics. */
bool VertexBuffer::addBuffer(ArrayType type)
{
  u32 offset = 0;
  u32 index = 0;

  SBUFFERSEMANTIC semantic;
  
  // go thru all semantics
  for (std::vector<SBUFFERSEMANTIC>::iterator iter = m_semantics.begin(); iter != m_semantics.end(); ++iter)
  {
    // calculate according to type
    switch (iter->type)
    {
      case VertexBuffer::ARRAY_TYPE_POSITION_XYZ:  offset += 3; break;
      case VertexBuffer::ARRAY_TYPE_COLOR_RGBA:    offset += 4; break;
      case VertexBuffer::ARRAY_TYPE_NORMAL:        offset += 3; break;
      case VertexBuffer::ARRAY_TYPE_TEXTURE_UV:    offset += 2; break;
      case VertexBuffer::ARRAY_TYPE_TANGENT:       offset += 3; break;
    }

    // check if the same array type is already in
    if (iter->type == type)
    {
      // increase index
      ++index;
    }
  }

  // set semantic data
  semantic.type   = type;
  semantic.offset = offset * sizeof(float32);
  semantic.index  = index;

  // add buffer semantic
  m_semantics.push_back(semantic);

  // reset cached vertex size
  m_vertexSize = 0;

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns number of arrays of given type in semantics. */
u32 VertexBuffer::bufferCount(ArrayType type) const
{
  u32 count = 0;
  for (std::vector<SBUFFERSEMANTIC>::const_iterator iter = m_semantics.begin(); iter != m_semantics.end(); ++iter)
  {
    if (iter->type == type)
    {
      ++count;
    }
  }

  return count;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates buffer for requested number of vertices. */
bool VertexBuffer::create(u32 count)
{
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

  // store data
  m_locked = false;

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Destroys buffer. */
void VertexBuffer::destroy()
{
  m_buffer = NULL;

  m_locked = false;

  m_vertexSize = 0;

  m_semantics.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Locks buffer given part of the buffer for read/write operations. */
void* VertexBuffer::lock(u32 offset, u32 count)
{
  // check if NOT locked yet and any data to lock
  if (!m_locked && (0 < count))
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

    // set lock flag
    m_locked = true;

    // update amount of data used
    m_buffer->setSize((offset + count) * vertexSize());

    return reinterpret_cast<u8*>(m_buffer->data()) + offset * vertexSize();
  }

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Unlocks buffer. */
void VertexBuffer::unlock()
{
  m_locked = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns number of allocated vertices. */
u32 VertexBuffer::vertexCount() const
{
  return (m_buffer) ? static_cast<u32>(m_buffer->size() / vertexSize()) : 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns vertex size for current semantics (in bytes). */
u32 VertexBuffer::vertexSize() const
{
  // check if need to calculate
  if (0 == m_vertexSize)
  {
    // go thru all buffers
    for (std::vector<SBUFFERSEMANTIC>::const_iterator iter = m_semantics.begin(); iter != m_semantics.end(); ++iter)
    {
      switch (iter->type)
      {
        case VertexBuffer::ARRAY_TYPE_POSITION_XYZ:  m_vertexSize += 3; break;
        case VertexBuffer::ARRAY_TYPE_COLOR_RGBA:    m_vertexSize += 4; break;
        case VertexBuffer::ARRAY_TYPE_NORMAL:        m_vertexSize += 3; break;
        case VertexBuffer::ARRAY_TYPE_TEXTURE_UV:    m_vertexSize += 2; break;
        case VertexBuffer::ARRAY_TYPE_TANGENT:       m_vertexSize += 3; break;
      }
    }

    m_vertexSize *= sizeof(float32);
  }

  return m_vertexSize;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns current buffer semantics. */
const std::vector<VertexBuffer::SBUFFERSEMANTIC>& VertexBuffer::semantics() const
{
  return m_semantics; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

//bool VertexBuffer::CompareBufferSemantics( PVertexBuffer& cVertexBuffer1, PVertexBuffer& cVertexBuffer2 )
//{
//  // check if semantics buffers are of different size
//  if ( cVertexBuffer1->m_vsBufferSemantics.size() != cVertexBuffer2->m_vsBufferSemantics.size() )
//  {
//    // not the same
//    return false;
//  }
//
//  // go thru all semantics
//  for ( u32 uiSemantic = 0; uiSemantic < static_cast<u32>( cVertexBuffer1->m_vsBufferSemantics.size() ); uiSemantic++ )
//  {
//    // check if NOT the same
//    if ( cVertexBuffer1->getBufferSemantics()[ uiSemantic ].eType != 
//         cVertexBuffer2->getBufferSemantics()[ uiSemantic ].eType )
//    {
//      // not the same
//      return false;
//    }
//  }
//
//  // semantics are the same
//  return true;
//}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

//u32 VertexBuffer::getBufferOffset( EType eBufferType ) const
//{
//  // go thru all buffers
//  for ( BufferSemanticVector::const_iterator iter = m_vsBufferSemantics.begin(); 
//        iter != m_vsBufferSemantics.end(); ++iter )
//  {
//    // check if found
//    if ( iter->eType == eBufferType )
//    {
//      return iter->uiOffset;
//    }
//  }
//
//  // requested buffer has not been found
//  return 0;
//}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Reallocates internal buffer to accomodate given number of indicies. */
bool VertexBuffer::reallocateBuffer(u32 count)
{
  // check if not allocated yet
  if (NULL == m_buffer)
  {
    // allocate buffer
    m_buffer = ege_new DataBuffer();
    if (NULL == m_buffer)
    {
      // error!
      return false;
    }
  }

  // allocate buffer for requested indicies
  if (EGE_SUCCESS != m_buffer->setSize(vertexSize() * count))
  {
    // error!
    return false;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
