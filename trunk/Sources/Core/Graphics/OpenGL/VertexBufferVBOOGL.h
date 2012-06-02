#ifndef EGE_CORE_VERTEXBUFFERVBOOGL_H
#define EGE_CORE_VERTEXBUFFERVBOOGL_H

/** 
 *   VertexBuffer base class specializtion based on Vertex Buffer Objects extension.
 */

#include <EGE.h>
#include "Core/Graphics/VertexBuffer.h"
#include <EGEOpenGL.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(DataBuffer, PDataBuffer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class VertexBufferVBO : public VertexBuffer
{
  public:

    VertexBufferVBO(Application* app, EGEVertexBuffer::UsageType usage);
   ~VertexBufferVBO();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /* VertexBuffer override. Returns TRUE if object is valid. */
    bool isValid() const override;
    /* VertexBuffer override. Sets buffer to given size. 
     * @param count Number of vertices buffer should contain.
     * @return Returns TRUE if success. Otherwise, FALSE.
     */
    bool setSize(u32 count) override;
    /* VertexBuffer override. Locks buffer's given part of the buffer for read/write operations. 
     * @param offset  0-based vertex offset from which locking should be done. 
     * @param count   Number of vertices to lock.
     */
    void* lock(u32 offset, u32 count) override;
    /* VertexBuffer override. Unlocks buffer. */
    void unlock(void* data) override;

    /*! VertexBuffer override. Returns number of allocated vertices. */
    u32 vertexCount() const override { return m_vertexCount; }
    /* VertexBuffer override. Returns maximal number of available vertices. */
    u32 vertexCapacity() const override { return m_vertexCapacity; }

    /* Binds buffer. */
    bool bind();
    /* Unbinds buffer. */
    bool unbind();

  private:

    /* VertexBuffer override. Destroys buffer. */
    void destroy() override;
    /* Reallocates internal buffer to accomodate given number of vertices. */
    bool reallocateBuffer(u32 count);

  private:

    /*! OpenGL VBO identifier. */
    GLuint m_id;
    /*! Vertex count. */
    u32 m_vertexCount;
    /*! Vertex capacity. */
    u32 m_vertexCapacity;
    /*! Shadow data buffer. */
    PDataBuffer m_shadowBuffer;  
    /*! Flag indicating if current lock is done on shadow buffer directly. */
    bool m_shadowBufferLock;
    /*! Lock offset (in vertices). */
    u32 m_lockOffset;
    /*! Lock length (in vertices). */
    u32 m_lockLength;
    /*! Map pointer. Only valid if mapping API is used for ie. m_shadowBufferLock is FALSE. */
    void* m_mapping;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_VERTEXBUFFERVBOOGL_H
