#include "Core/Graphics/VertexDeclaration.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VertexDeclaration::VertexDeclaration() : m_vertexSize(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VertexDeclaration::~VertexDeclaration()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VertexDeclaration& VertexDeclaration::operator = (const VertexDeclaration& other)
{
  if (this != &other)
  {
    m_vertexElements  = other.m_vertexElements;
    m_vertexSize      = other.m_vertexSize;
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool VertexDeclaration::operator == (const VertexDeclaration& other) const
{
  return (m_vertexSize == other.m_vertexSize) && (m_vertexElements == other.m_vertexElements);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool VertexDeclaration::operator != (const VertexDeclaration& other) const
{
  return ! (*this == other);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool VertexDeclaration::addElement(NVertexBuffer::VertexElementSemantic semantic)
{
  u32 offset = 0;
  u32 index = 0;
 
  // go thru all semantics
  for (VertexElementArray::const_iterator iter = m_vertexElements.begin(); iter != m_vertexElements.end(); ++iter)
  {
    // calculate according to type
    switch (iter->semantic())
    {
      case NVertexBuffer::VES_POSITION_XYZ:       offset += 3; break;
      case NVertexBuffer::VES_POSITION_XY:        offset += 2; break;
      case NVertexBuffer::VES_COLOR_RGBA:         offset += 4; break;
      case NVertexBuffer::VES_NORMAL:             offset += 3; break;
      case NVertexBuffer::VES_TEXTURE_UV:         offset += 2; break;
      case NVertexBuffer::VES_TANGENT:            offset += 3; break;
      case NVertexBuffer::VES_POINT_SPRITE_SIZE:  offset += 1; break;
        
      default:
        
        EGE_ASSERT_X(false, "Unknown array type");
        break;
    }

    // check if the same array type is already in
    if (iter->semantic() == semantic)
    {
      // increase index
      ++index;
    }
  }

  // add to pool
  m_vertexElements.push_back(VertexElement(semantic, offset * sizeof(float32), index));

  // reset cached vertex size
  m_vertexSize = 0;

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 VertexDeclaration::elementCount(NVertexBuffer::VertexElementSemantic semantic) const
{
  s32 count = 0;
  for (VertexElementArray::const_iterator iter = m_vertexElements.begin(); iter != m_vertexElements.end(); ++iter)
  {
    if (semantic == iter->semantic())
    {
      ++count;
    }
  }

  return count;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 VertexDeclaration::vertexSize() const
{
  // check if need to calculate
  if (0 == m_vertexSize)
  {
    // go thru all buffers
    for (VertexElementArray::const_iterator iter = m_vertexElements.begin(); iter != m_vertexElements.end(); ++iter)
    {
      switch (iter->semantic())
      {
        case NVertexBuffer::VES_POSITION_XYZ:        m_vertexSize += 3; break;
        case NVertexBuffer::VES_POSITION_XY:         m_vertexSize += 2; break;
        case NVertexBuffer::VES_COLOR_RGBA:          m_vertexSize += 4; break;
        case NVertexBuffer::VES_NORMAL:              m_vertexSize += 3; break;
        case NVertexBuffer::VES_TEXTURE_UV:          m_vertexSize += 2; break;
        case NVertexBuffer::VES_TANGENT:             m_vertexSize += 3; break;
        case NVertexBuffer::VES_POINT_SPRITE_SIZE:   m_vertexSize += 1; break;
          
        default:
          
          EGE_ASSERT_X(false, "Unknown array type");
          break;
      }
    }

    m_vertexSize *= sizeof(float32);
  }

  return m_vertexSize;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const VertexElementArray& VertexDeclaration::vertexElements() const
{
  return m_vertexElements; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void VertexDeclaration::clear()
{
  m_vertexSize = 0;
  m_vertexElements.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const VertexElement* VertexDeclaration::vertexElement(NVertexBuffer::VertexElementSemantic semantic, u32 index) const
{
  const VertexElement* element = NULL;

  // go thru all elements
  for (VertexElementArray::const_iterator iter = m_vertexElements.begin(); iter != m_vertexElements.end(); ++iter)
  {
    const VertexElement& current = *iter;

    // check if found
    if ((semantic == current.semantic()) && (index == current.index()))
    {
      // found
      element = &current;
      break;
    }
  }

  return element;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
NVertexBuffer::VertexElementSemantic VertexDeclaration::findPositionSemantic() const
{
  NVertexBuffer::VertexElementSemantic semantic = NVertexBuffer::VES_NONE;

  // go thru all elements
  for (VertexElementArray::const_iterator iter = m_vertexElements.begin(); iter != m_vertexElements.end(); ++iter)
  {
    const VertexElement& current = *iter;

    // check if found
    if ((NVertexBuffer::VES_POSITION_XY == current.semantic()) || (NVertexBuffer::VES_POSITION_XYZ == current.semantic()))
    {
      // found
      semantic = current.semantic();
      break;
    }
  }

  return semantic;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END