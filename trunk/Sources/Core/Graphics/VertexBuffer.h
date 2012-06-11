#ifndef EGE_CORE_VERTEXBUFFER_H
#define EGE_CORE_VERTEXBUFFER_H

/** 
 *   VertexBuffer base class. Instances of this class are used to define vertex array of arbitrary type(s).
 *   Vertex arrays data are stored one after another (interleaved)
 */

#include <EGE.h>
#include <EGEList.h>
#include "Core/Graphics/VertexBufferTypes.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(VertexBuffer, PVertexBuffer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class VertexBuffer : public Object
{
  public:

    VertexBuffer(Application* app, u32 uid, EGEVertexBuffer::UsageType usage);
    virtual ~VertexBuffer();

    /*! Returns TRUE if object is valid. */
    virtual bool isValid() const = 0;
    /*! Sets buffer to given size. 
     * @param count Number of vertices buffer should contain.
     * @return Returns TRUE if success. Otherwise, FALSE.
     */
    virtual bool setSize(u32 count) = 0;

    /*! Locks buffer's given part of the buffer for read/write operations. 
     * @param offset  0-based vertex offset from which locking should be done. 
     * @param count   Number of vertices to lock.
     */
    virtual void* lock(u32 offset, u32 count) = 0;
    /*! Unlocks buffer. */
    virtual void unlock(void* data) = 0;

    /* Adds given array type to overall semantics. */
    bool addArray(EGEVertexBuffer::ArrayType type);
    /* Sets semantics to given type. */
    bool setSemantics(EGEVertexBuffer::SemanticType type);
    /* Sets semantics to a given set of array types. */
    bool setSemantics(const List<EGEVertexBuffer::ArrayType>& types);
    /* Returns number of arrays of given type in semantics. */
    s32 arrayCount(EGEVertexBuffer::ArrayType type) const;

    /* Returns current array semantics. */
    const EGEVertexBuffer::SemanticArray& semantics() const;

    /*! Returns number of vertices currently in use. */
    virtual u32 vertexCount() const = 0;
    /*! Returns maximal number of available vertices. */
    virtual u32 vertexCapacity() const = 0;
    /* Returns vertex size for current semantics (in bytes). */
    u32 vertexSize() const;

  protected:

    /* Destroys buffer. */
    virtual void destroy();

  protected:

    /*! TRUE if buffer is locked. */
    bool m_locked;
    /*! Cached vertex size (in bytes), 0 if not calculated yet. */
    mutable u32 m_vertexSize;
    /*! Buffer semantics. */
    EGEVertexBuffer::SemanticArray m_semantics;
    /*! Usage. */
    EGEVertexBuffer::UsageType m_usage;

  private:

    /* Clears vertex data. */
    virtual void clear();
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_VERTEXBUFFER_H
