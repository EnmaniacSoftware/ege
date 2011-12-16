#ifndef EGE_CORE_INDEXBUFFER_H
#define EGE_CORE_INDEXBUFFER_H

/** 
 *   IndexBuffer base class. Instances of this class are used to define index array to vertex buffers.
 */

#include <EGE.h>
#include <EGEIndexBuffer.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(IndexBuffer, PIndexBuffer)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class IndexBuffer : public Object
{
  public:

    IndexBuffer(Application* app, u32 uid);
    virtual ~IndexBuffer();

    /* Returns TRUE if object is valid. */
    virtual bool isValid() const = 0;
    /* Creates buffer for requested number of indicies of given size. */
    virtual bool create(EGEIndexBuffer::IndexSize size, u32 count = 0);

    /* Locks buffer's given part of the buffer for read/write operations. 
     * @param offset  0-based index offset from which locking should be done. 
     * @param count   Number of indicies to lock.
     */
    virtual void* lock(u32 offset, u32 count) = 0;
    /* Unlocks buffer. */
    virtual void unlock() = 0;

    /* Returns number of allocated indicies. */
    virtual u32 indexCount() const = 0;
    /* Returns index size (in bytes). */
    u8 indexSize() const;

    /*! Returns size type. */
    inline EGEIndexBuffer::IndexSize size() const { return m_size; }
 
  protected:

    /* Destroys buffer. */
    virtual void destroy();

  protected:

    /*! Size of indicies. */
    EGEIndexBuffer::IndexSize m_size;
    /*! TRUE if buffer is locked. */
    bool m_locked;
    /*! Usage. */
    EGEIndexBuffer::UsageType m_usage;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_INDEXBUFFER_H