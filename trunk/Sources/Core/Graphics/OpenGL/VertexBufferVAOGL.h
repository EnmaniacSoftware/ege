#ifndef EGE_CORE_VERTEXBUFFERVAOGL_H
#define EGE_CORE_VERTEXBUFFERVAOGL_H

/** 
*   VertexBuffer is used to define single vertex lookup and to keep certain amount of such vertices in memory
*   Vertex data is stored one after another (interleaved)
*/

#include <EGE.h>
#include "Core/Graphics/VertexBuffer.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(DataBuffer, PDataBuffer)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class VertexBufferVA : public VertexBuffer
{
  public:

    VertexBufferVA(Application* app);
    virtual ~VertexBufferVA();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /* VertexBuffer override. Returns TRUE if object is valid. */
    bool isValid() const override;
    /* VertexBuffer override. Creates buffer for requested number of vertices. */
    bool create(u32 count) override;
    /* VertexBuffer override. Locks buffer's given part of the buffer for read/write operations. 
     * @param offset  0-based vertex offset from which locking should be done. 
     * @param count   Number of vertices to lock.
     */
    void* lock(u32 offset, u32 count) override;
    /* VertexBuffer override. Unlocks buffer. */
    void unlock() override;

    /* VertexBuffer override. Returns number of allocated vertices. */
    u32 vertexCount() const override;

  private:

    /* VertexBuffer override. Destroys buffer. */
    void destroy() override;
    /* Reallocates internal buffer to accomodate given number of vertices. */
    bool reallocateBuffer(u32 count);

  private:

    /*! Data buffer. */
    PDataBuffer m_buffer;         
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_VERTEXBUFFERVAOGL_H
