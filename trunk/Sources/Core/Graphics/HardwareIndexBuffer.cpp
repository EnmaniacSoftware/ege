#include "Core/Graphics/HardwareIndexBuffer.h"

#if EGE_RENDERING_OPENGL_2 || EGE_RENDERING_OPENGLES_1
//#include "Core/Graphics/OpenGL/HardwareVertexBufferOGL.h"
#endif // EGE_RENDERING_OPENGL_2 || EGE_RENDERING_OPENGLES_1

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(HardwareIndexBuffer)
EGE_DEFINE_DELETE_OPERATORS(HardwareIndexBuffer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//HardwareIndexBuffer::HardwareIndexBuffer(Application* app, EGEVertexBuffer::UsageType usage) : VertexBuffer(app, EGE_OBJECT_UID_HARDWARE_VERTEX_BUFFER), 
//                                                                                                 m_usage(usage),
//                                                                                                 m_vertexCount(0)
//{
//  m_p = ege_new HardwareVertexBufferPrivate(this);
//}
////--------------------------------------------------------------------------------------------------------------------------------------------------------------
//HardwareIndexBuffer::~HardwareIndexBuffer()
//{
//  EGE_DELETE(m_p);
//}
////--------------------------------------------------------------------------------------------------------------------------------------------------------------
///*! Returns TRUE if object is valid. */
//bool HardwareIndexBuffer::isValid() const
//{
//  return (NULL != m_p) && m_p->isValid();
//}
////--------------------------------------------------------------------------------------------------------------------------------------------------------------
///*! VertexBuffer override. Creates buffer for requested number of vertices. */
//bool HardwareIndexBuffer::create(u32 count)
//{
//  if (isValid())
//  {
//    return p_func()->create(count);
//  }
//
//  return false;
//}
////--------------------------------------------------------------------------------------------------------------------------------------------------------------
///*! VertexBuffer override. Locks buffer's given part of the buffer for read/write operations. 
// * @param offset  0-based vertex offset from which locking should be done. 
// * @param count   Number of vertices to lock.
// */
//void* HardwareIndexBuffer::lock(u32 offset, u32 count)
//{
//  if (isValid())
//  {
//    return p_func()->lock(offset, count);
//  }
//
//  return NULL;
//}
////--------------------------------------------------------------------------------------------------------------------------------------------------------------
///*! VertexBuffer override. Unlocks buffer. */
//void HardwareIndexBuffer::unlock()
//{
//  if (isValid())
//  {
//    return p_func()->unlock();
//  }
//}
////--------------------------------------------------------------------------------------------------------------------------------------------------------------
///*! VertexBuffer override. Reallocates internal buffer to accomodate given number of vertices. */
//bool HardwareVertexBuffer::reallocateBuffer(u32 count)
//{
//  if (isValid())
//  {
//    return p_func()->reallocateBuffer(count);
//  }
//
//  return false;
//}
////--------------------------------------------------------------------------------------------------------------------------------------------------------------
///*! VertexBuffer override. Binds buffer. */
//bool HardwareIndexBuffer::bind()
//{
//  if (isValid())
//  {
//    return p_func()->bind();
//  }
//
//  return false;
//}
////--------------------------------------------------------------------------------------------------------------------------------------------------------------
///*! VertexBuffer override. Unbinds buffer. */
//bool HardwareIndexBuffer::unbind()
//{
//  if (isValid())
//  {
//    return p_func()->unbind();
//  }
//
//  return false;
//}
////--------------------------------------------------------------------------------------------------------------------------------------------------------------
