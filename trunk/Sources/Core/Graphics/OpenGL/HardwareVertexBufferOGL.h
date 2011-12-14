#ifndef EGE_CORE_HARDWAREVERTEXBUFFER_PRIVATE_H
#define EGE_CORE_HARDWAREVERTEXBUFFER_PRIVATE_H

#include <EGE.h>
#include <EGEVertexBuffer.h>
#include <EGEOpenGL.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class HardwareVertexBuffer;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class HardwareVertexBufferPrivate
{
  public:

    HardwareVertexBufferPrivate(HardwareVertexBuffer* base);
   ~HardwareVertexBufferPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Creates buffer for requested number of vertices. */
    bool create(u32 count);
    /* Locks buffer's given part of the buffer for read/write operations. 
     * @param offset  0-based vertex offset from which locking should be done. 
     * @param count   Number of vertices to lock.
     */
    void* lock(u32 offset, u32 count);
    /* Unlocks buffer. */
    void unlock();

    /* Reallocates internal buffer to accomodate given number of vertices. */
    bool reallocateBuffer(u32 count);

    /* Binds buffer. */
    bool bind();
    /* Unbinds buffer. */
    bool unbind();

    /* Returns number of allocated vertices. */
    u32 vertexCount() const;

  private:

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(HardwareVertexBuffer);

    /*! OpenGL VBO identifier. */
    GLuint m_id;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_HARDWAREVERTEXBUFFER_PRIVATE_H
