#include "Core/Graphics/VertexBuffer.h"
#include "Core/Data/DataBuffer.h"
#include <EGEList.h>
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(VertexBuffer)
EGE_DEFINE_DELETE_OPERATORS(VertexBuffer)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VertexBuffer::VertexBuffer(Application* app) : Object(app), m_locked(false), m_vertexSize(0)
{
  m_buffer = ege_new DataBuffer();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VertexBuffer::~VertexBuffer()
{
  destroy();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool VertexBuffer::isValid() const
{
  return NULL != m_buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds given array type to overall semantics. */
bool VertexBuffer::addArray(EGEVertexBuffer::ArrayType type)
{
  u32 offset = 0;
  u32 index = 0;

  EGEVertexBuffer::SARRAYSEMANTIC semantic;
  
  // go thru all semantics
  for (SemanticsList::const_iterator iter = m_semantics.begin(); iter != m_semantics.end(); ++iter)
  {
    // calculate according to type
    switch (iter->type)
    {
      case EGEVertexBuffer::AT_POSITION_XYZ:       offset += 3; break;
      case EGEVertexBuffer::AT_COLOR_RGBA:         offset += 4; break;
      case EGEVertexBuffer::AT_NORMAL:             offset += 3; break;
      case EGEVertexBuffer::AT_TEXTURE_UV:         offset += 2; break;
      case EGEVertexBuffer::AT_TANGENT:            offset += 3; break;
      case EGEVertexBuffer::AT_POINT_SPRITE_SIZE:  offset += 1; break;
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
u32 VertexBuffer::arrayCount(EGEVertexBuffer::ArrayType type) const
{
  u32 count = 0;
  for (SemanticsList::const_iterator iter = m_semantics.begin(); iter != m_semantics.end(); ++iter)
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

  clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Locks buffer's given part of the buffer for read/write operations. 
 *  @param offset  0-based vertex offset from which locking should be done. 
 *  @param count   Number of vertices to lock.
 */
void* VertexBuffer::lock(u32 offset, u32 count)
{
  // check if NOT locked yet and any data to lock
  if (!m_locked && (0 <= count))
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
    for (SemanticsList::const_iterator iter = m_semantics.begin(); iter != m_semantics.end(); ++iter)
    {
      switch (iter->type)
      {
        case EGEVertexBuffer::AT_POSITION_XYZ:       m_vertexSize += 3; break;
        case EGEVertexBuffer::AT_COLOR_RGBA:          m_vertexSize += 4; break;
        case EGEVertexBuffer::AT_NORMAL:              m_vertexSize += 3; break;
        case EGEVertexBuffer::AT_TEXTURE_UV:          m_vertexSize += 2; break;
        case EGEVertexBuffer::AT_TANGENT:             m_vertexSize += 3; break;
        case EGEVertexBuffer::AT_POINT_SPRITE_SIZE:   m_vertexSize += 1; break;
      }
    }

    m_vertexSize *= sizeof(float32);
  }

  return m_vertexSize;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns current buffer semantics. */
const VertexBuffer::SemanticsList& VertexBuffer::semantics() const
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
  // allocate buffer for requested indicies
  if (EGE_SUCCESS != m_buffer->setSize(vertexSize() * count))
  {
    // error!
    return false;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets semantics to given type. */
bool VertexBuffer::setSemantics(EGEVertexBuffer::SemanticType type)
{
  // clear any previous semantics
  clear();

  // process according to type
  List<EGEVertexBuffer::ArrayType> arrayList;
  switch (type)
  {
    case EGEVertexBuffer::ST_V3:

      arrayList.push_back(EGEVertexBuffer::AT_POSITION_XYZ);
      break;

    case EGEVertexBuffer::ST_V3_T2:

      arrayList.push_back(EGEVertexBuffer::AT_POSITION_XYZ);
      arrayList.push_back(EGEVertexBuffer::AT_TEXTURE_UV);
      break;

    case EGEVertexBuffer::ST_V3_T2_C4:

      arrayList.push_back(EGEVertexBuffer::AT_POSITION_XYZ);
      arrayList.push_back(EGEVertexBuffer::AT_TEXTURE_UV);
      arrayList.push_back(EGEVertexBuffer::AT_COLOR_RGBA);
      break;

    case EGEVertexBuffer::ST_V3_C4:

      arrayList.push_back(EGEVertexBuffer::AT_POSITION_XYZ);
      arrayList.push_back(EGEVertexBuffer::AT_COLOR_RGBA);
      break;

    default:

      EGE_ASSERT(false && "Implement?");
  }

  // add all array types in order
  for (List<EGEVertexBuffer::ArrayType>::const_iterator it = arrayList.begin(); it != arrayList.end(); ++it)
  {
    // add current array type
    if (!addArray(*it))
    {
      // error!
      return false;
    }
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Clears vertex data. */
void VertexBuffer::clear()
{
  m_locked = false;

  m_vertexSize = 0;

  m_semantics.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
