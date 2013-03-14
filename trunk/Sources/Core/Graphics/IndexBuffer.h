#ifndef EGE_CORE_GRAPHICS_INDEXBUFFER_H
#define EGE_CORE_GRAPHICS_INDEXBUFFER_H

/** 
 *   IndexBuffer base class. Instances of this class are used to define index array to vertex buffers.
 */

#include "EGE.h"
#include "EGEIndexBuffer.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(IndexBuffer, PIndexBuffer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IndexBuffer : public Object
{
  public:

    virtual ~IndexBuffer();

    /*! Returns TRUE if object is valid. */
    virtual bool isValid() const = 0;
    /*! Sets buffer to given size. 
     *  @param count Number of indicies buffer should contain.
     *  @return Returns TRUE if success. Otherwise, FALSE.
     */
    virtual bool setSize(u32 count) = 0;

    /*! Locks buffer's given part of the buffer for read/write operations. 
     *  @param offset  0-based index offset from which locking should be done. 
     *  @param count   Number of indicies to lock.
     *  @return Pointer to the begining of the locked part of the buffer. NULL if lock could not be set.
     *  @note Locking fails if requested block lies outside of the buffer.
     */
    virtual void* lock(u32 offset, u32 count) = 0;
    /*! Unlocks buffer. */
    virtual void unlock(void* data) = 0;

    /*! Returns number of allocated indicies. */
    virtual u32 indexCount() const = 0;

    /*! Returns index size (in bytes). */
    u8 indexSize() const;

    /*! Sets index size. */
    void setIndexSize(EGEIndexBuffer::IndexSize size);
    /*! Returns size type. */
    EGEIndexBuffer::IndexSize size() const { return m_indexSize; }
 
  protected:

    /*! Constructing only via RenderSystem. */
    IndexBuffer(Application* app, u32 uid);
    /*! Destroys buffer. */
    virtual void destroy();

  protected:

    /*! Size of indicies. */
    EGEIndexBuffer::IndexSize m_indexSize;
    /*! TRUE if buffer is locked. */
    bool m_locked;
    /*! Usage. */
    EGEIndexBuffer::UsageType m_usage;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_INDEXBUFFER_H