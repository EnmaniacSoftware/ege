#ifndef EGE_CORE_VERTEXBUFFER_H
#define EGE_CORE_VERTEXBUFFER_H

/** 
*   VertexBuffer is used to define single vertex lookup and to keep certain amount of such vertices in memory
*   Vertex data is stored one after another (interleaved)
*/

#include <EGE.h>
#include <EGEDynamicArray.h>
#include "Core/Graphics/VertexBufferTypes.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(VertexBuffer, PVertexBuffer)
EGE_DECLARE_SMART_CLASS(DataBuffer, PDataBuffer)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class VertexBuffer : public Object
{
  public:

    VertexBuffer(Application* app);
    virtual ~VertexBuffer();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    typedef DynamicArray<EGEVertexBuffer::SARRAYSEMANTIC> SemanticsList;

  public:

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Creates buffer for requested number of vertices. */
    virtual bool create(u32 count);
    /* Locks buffer's given part of the buffer for read/write operations. 
     * @param offset  0-based vertex offset from which locking should be done. 
     * @param count   Number of vertices to lock.
     */
    virtual void* lock(u32 offset, u32 count);
    /* Unlocks buffer. */
    virtual void unlock();

    /* Adds given array type to overall semantics. */
    bool addArray(EGEVertexBuffer::ArrayType type);
    /* Sets semantics to given type. */
    bool setSemantics(EGEVertexBuffer::SemanticType type);
    /* Returns number of arrays of given type in semantics. */
    u32 arrayCount(EGEVertexBuffer::ArrayType type) const;

    /* Returns current array semantics. */
    const SemanticsList& semantics() const;

    /* Returns number of allocated vertices. */
    virtual u32 vertexCount() const;
    /* Returns vertex size for current semantics (in bytes). */
    u32 vertexSize() const;

    /* Binds buffer. */
    virtual bool bind();
    /* Unbinds buffer. */
    virtual bool unbind();

  protected:

    VertexBuffer(Application* app, u32 uid);
    /* Reallocates internal buffer to accomodate given number of vertices. */
    virtual bool reallocateBuffer(u32 count);

  protected:

    /*! TRUE if buffer is locked. */
    bool m_locked;

  private:

    /* Destroys buffer. */
    void destroy();
    /* Clears vertex data. */
    void clear();

  private:

    /*! Data buffer. */
    PDataBuffer m_buffer;         
    /*! Cached vertex size (in bytes), 0 if not calculated yet. */
    mutable u32 m_vertexSize;
    /*! Buffer semantics. */
    SemanticsList m_semantics;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_VERTEXBUFFER_H
