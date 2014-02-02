#include "Core/Graphics/VertexElement.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VertexElement::VertexElement(NVertexBuffer::VertexElementSemantic semantic, u32 offset, u32 index) : m_semantic(semantic)
                                                                                                   , m_offset(offset)
                                                                                                   , m_index(index)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VertexElement::~VertexElement()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool VertexElement::operator == (const VertexElement& other) const
{
  return (m_semantic == other.m_semantic) && (m_offset == other.m_offset) && (m_index == other.m_index);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
NVertexBuffer::VertexElementSemantic VertexElement::semantic() const
{
  return m_semantic;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 VertexElement::offset() const
{
  return m_offset;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 VertexElement::index() const
{
  return m_index;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 VertexElement::size() const
{
  u32 size = 0;

  switch (semantic())
  {
    case NVertexBuffer::VES_POSITION_XYZ:        size += 3; break;
    case NVertexBuffer::VES_POSITION_XY:         size += 2; break;
    case NVertexBuffer::VES_COLOR_RGBA:          size += 4; break;
    case NVertexBuffer::VES_NORMAL:              size += 3; break;
    case NVertexBuffer::VES_TEXTURE_UV:          size += 2; break;
    case NVertexBuffer::VES_TANGENT:             size += 3; break;
    case NVertexBuffer::VES_POINT_SPRITE_SIZE:   size += 1; break;
          
    default:
          
      EGE_ASSERT_X(false, "Unknown array type");
      break;
  }

  size *= sizeof (float32);
  return size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END