#include "Core/Graphics/VertexBuffer.h"
#include "EGEDataBuffer.h"
#include "EGEList.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VertexBuffer::VertexBuffer(const String& name, const VertexDeclaration& vertexDeclaration) : Component(EGE_OBJECT_UID_VERTEX_BUFFER, name)
                                                                                           , m_locked(false)
                                                                                           , m_vertexDeclaration(vertexDeclaration)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VertexBuffer::~VertexBuffer()
{
  destroy();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const VertexDeclaration& VertexBuffer::vertexDeclaration() const
{
  return m_vertexDeclaration;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void VertexBuffer::destroy()
{
  clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void VertexBuffer::clear()
{
  m_locked = false;

  m_vertexDeclaration.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END