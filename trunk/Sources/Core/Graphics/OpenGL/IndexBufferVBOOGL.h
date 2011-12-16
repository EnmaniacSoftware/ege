#ifndef EGE_CORE_INDEXBUFFERVBOOGL_H
#define EGE_CORE_INDEXBUFFERVBOOGL_H

/** 
 *   IndexBuffer base class specialization based on Vertex Buffer Objects extension.
 */

#include <EGE.h>
#include "Core/Graphics/IndexBuffer.h"
#include <EGEOpenGL.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class IndexBufferVBO : public IndexBuffer
{
  public:

    IndexBufferVBO(Application* app);
   ~IndexBufferVBO();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /* IndexBuffer override. Returns TRUE if object is valid. */
    bool isValid() const override;
    /* IndexBuffer override. Creates buffer for requested number of vertices. */
    bool create(EGEIndexBuffer::IndexSize size, u32 count) override;
    /* IndexBuffer override. Locks buffer's given part of the buffer for read/write operations. 
     * @param offset  0-based index offset from which locking should be done. 
     * @param count   Number of indicies to lock.
     */
    void* lock(u32 offset, u32 count) override;
    /* IndexBuffer override. Unlocks buffer. */
    void unlock() override;

    /*! IndexBuffer override. Returns number of allocated vertices. */
    u32 indexCount() const override { return m_indexCount; }

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
    /*! Index count. */
    u32 m_indexCount;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_INDEXBUFFERVBOOGL_H
