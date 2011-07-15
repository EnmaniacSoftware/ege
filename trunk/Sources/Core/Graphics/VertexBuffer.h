#ifndef EGE_CORE_VERTEXBUFFER_H
#define EGE_CORE_VERTEXBUFFER_H

/** 
*   VertexBuffer is used to define single vertex lookup and to keep certain amount of such vertices in memory
*   Vertex data is stored one after another (interleaved)
*/

#include <EGE.h>
#include <EGEVertexBuffer.h>
#include <EGEDynamicArray.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(VertexBuffer, PVertexBuffer)
EGE_DECLARE_SMART_CLASS(DataBuffer, PDataBuffer)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class VertexBuffer : public Object
{
  public:

    VertexBuffer(Application* app);
   ~VertexBuffer();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Creates buffer for requested number of vertices. */
    bool create(u32 count);
    /* Destroys buffer. */
    void destroy();
    /* Locks buffer given part of the buffer for read/write operations. */
    void* lock(u32 offset, u32 count);
    /* Unlocks buffer. */
    void unlock();

    /* Adds given array type to overall semantics. */
    bool addArray(EGEVertexBuffer::ArrayType type);
    /* Returns number of arrays of given type in semantics. */
    u32 arrayCount(EGEVertexBuffer::ArrayType type) const;

    /* Returns current array semantics. */
    const DynamicArray<EGEVertexBuffer::SARRAYSEMANTIC>& semantics() const;

    /* Returns number of allocated vertices. */
    u32 vertexCount() const;
    /* Returns vertex size for current semantics (in bytes). */
    u32 vertexSize() const;

  private:

    /* Reallocates internal buffer to accomodate given number of vertices. */
    bool reallocateBuffer(u32 count);

  private:

    /*! TRUE if buffer is locked. */
    bool m_locked;
    /*! Data buffser. */
    PDataBuffer m_buffer;         
    /*! Cached vertex size (in bytes), 0 if not calculated yet. */
    mutable u32 m_vertexSize;
    /*! Buffer semantics. */
    DynamicArray<EGEVertexBuffer::SARRAYSEMANTIC> m_semantics;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_VERTEXBUFFER_H
