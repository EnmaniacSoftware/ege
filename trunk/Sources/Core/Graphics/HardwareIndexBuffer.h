#ifndef EGE_CORE_HARDWAREINDEXBUFFER_H
#define EGE_CORE_HARDWAREINDEXBUFFER_H

/** 
 *   HardwareIndexBuffer base class.
 */

#include <EGE.h>
#include "Core/Graphics/IndexBuffer.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(HardwareIndexBuffer, PHardwareIndexBuffer)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class HardwareIndexBuffer : public IndexBuffer
{
  public:

    //HardwareIndexBuffer(Application* app, EGEVertexBuffer::UsageType usage);
    virtual ~HardwareIndexBuffer();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

  //  /* Returns TRUE if object is valid. */
  //  bool isValid() const;
  //  /* VertexBuffer override. Creates buffer for requested number of vertices. */
  //  bool create(u32 count) override;
  //  /* VertexBuffer override. Locks buffer's given part of the buffer for read/write operations. 
  //   * @param offset  0-based vertex offset from which locking should be done. 
  //   * @param count   Number of vertices to lock.
  //   */
  //  void* lock(u32 offset, u32 count) override;
  //  /* VertexBuffer override. Unlocks buffer. */
  //  void unlock() override;

  //  /* VertexBuffer override. Binds buffer. */
  //  bool bind() override;
  //  /* VertexBuffer override. Unbinds buffer. */
  //  bool unbind() override;

  //  /* VertexBuffer override. Returns number of allocated vertices. */
  //  u32 vertexCount() const override { return m_vertexCount; }

  //private:

  //  /* VertexBuffer override. Reallocates internal buffer to accomodate given number of vertices. */
  //  bool reallocateBuffer(u32 count) override;

  //private:

  //  EGE_DECLARE_PRIVATE_IMPLEMENTATION(HardwareVertexBuffer);

  //  /*! Usage type. */
  //  EGEVertexBuffer::UsageType m_usage;
  //  /*! Vertex count. */
  //  u32 m_vertexCount;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_HARDWAREINDEXBUFFER_H
