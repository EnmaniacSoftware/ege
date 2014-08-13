#include "Core/Graphics/OpenGL/Implementation/VertexArrayObject.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VertexArrayObject::VertexArrayObject(const String& name) : Component(EGE_OBJECT_UID_VERTEX_ARRAY_OBJECT, name)
                                                         , m_id(0)
{
  glGenVertexArrays(1, &m_id);
  OGL_CHECK();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VertexArrayObject::~VertexArrayObject()
{
  if (0 != m_id)
  {
    glDeleteVertexArrays(1, &m_id);
    OGL_CHECK();
    m_id = 0;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void VertexArrayObject::bind()
{
  glBindVertexArray(m_id);
  OGL_CHECK();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void VertexArrayObject::unbind()
{
  glBindVertexArray(0);
  OGL_CHECK();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END