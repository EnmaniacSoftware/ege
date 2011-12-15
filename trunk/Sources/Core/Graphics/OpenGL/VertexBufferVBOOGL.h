#ifndef EGE_CORE_HARDWAREVERTEXBUFFER_PRIVATE_H
#define EGE_CORE_HARDWAREVERTEXBUFFER_PRIVATE_H

#include <EGE.h>
#include "Core/Graphics/VertexBuffer.h"
#include <EGEOpenGL.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class VertexBufferVBO : public VertexBuffer
{
  public:

    VertexBufferVBO(Application* app);
   ~VertexBufferVBO();

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

    /*! VertexBuffer override. Returns number of allocated vertices. */
    u32 vertexCount() const override { return m_vertexCount; }

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
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_HARDWAREVERTEXBUFFER_PRIVATE_H
