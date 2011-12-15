#include "Core/Graphics/VertexBuffer.h"
#include "Core/Data/DataBuffer.h"
#include <EGEList.h>
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VertexBuffer::VertexBuffer(Application* app, u32 uid) : Object(app, uid), 
                                                        m_locked(false), 
                                                        m_vertexSize(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VertexBuffer::~VertexBuffer()
{
  destroy();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds given array type to overall semantics. */
bool VertexBuffer::addArray(EGEVertexBuffer::ArrayType type)
{
  u32 offset = 0;
  u32 index = 0;

  EGEVertexBuffer::SARRAYSEMANTIC semantic;
  
  // go thru all semantics
  for (EGEVertexBuffer::SemanticArray::const_iterator iter = m_semantics.begin(); iter != m_semantics.end(); ++iter)
  {
    // calculate according to type
    switch (iter->type)
    {
      case EGEVertexBuffer::AT_POSITION_XYZ:       offset += 3; break;
      case EGEVertexBuffer::AT_POSITION_XY:        offset += 2; break;
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
  for (EGEVertexBuffer::SemanticArray::const_iterator iter = m_semantics.begin(); iter != m_semantics.end(); ++iter)
  {
    if (iter->type == type)
    {
      ++count;
    }
  }

  return count;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Destroys buffer. */
void VertexBuffer::destroy()
{
  clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns vertex size for current semantics (in bytes). */
u32 VertexBuffer::vertexSize() const
{
  // check if need to calculate
  if (0 == m_vertexSize)
  {
    // go thru all buffers
    for (EGEVertexBuffer::SemanticArray::const_iterator iter = m_semantics.begin(); iter != m_semantics.end(); ++iter)
    {
      switch (iter->type)
      {
        case EGEVertexBuffer::AT_POSITION_XYZ:        m_vertexSize += 3; break;
        case EGEVertexBuffer::AT_POSITION_XY:         m_vertexSize += 2; break;
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
const EGEVertexBuffer::SemanticArray& VertexBuffer::semantics() const
{
  return m_semantics; 
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

    case EGEVertexBuffer::ST_V2:

      arrayList.push_back(EGEVertexBuffer::AT_POSITION_XY);
      break;

    case EGEVertexBuffer::ST_V2_T2:

      arrayList.push_back(EGEVertexBuffer::AT_POSITION_XY);
      arrayList.push_back(EGEVertexBuffer::AT_TEXTURE_UV);
      break;

    case EGEVertexBuffer::ST_V2_T2_C4:

      arrayList.push_back(EGEVertexBuffer::AT_POSITION_XY);
      arrayList.push_back(EGEVertexBuffer::AT_TEXTURE_UV);
      arrayList.push_back(EGEVertexBuffer::AT_COLOR_RGBA);
      break;

    case EGEVertexBuffer::ST_V2_C4:

      arrayList.push_back(EGEVertexBuffer::AT_POSITION_XY);
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
