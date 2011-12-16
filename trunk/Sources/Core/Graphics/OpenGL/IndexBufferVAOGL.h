#ifndef EGE_CORE_INDEXBUFFERVAOGL_H
#define EGE_CORE_INDEXBUFFERVAOGL_H

/** 
 *   IndexBuffer base class specialization based on Vertex Arrays.
 */

#include <EGE.h>
#include <EGEIndexBuffer.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(DataBuffer, PDataBuffer)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class IndexBufferVA : public IndexBuffer
{
  public:

    IndexBufferVA(Application* app);
    virtual ~IndexBufferVA();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* IndexBuffer override. Returns TRUE if object is valid. */
    bool isValid() const override;
    /* IndexBuffer override. Creates buffer for requested number of indicies of given size. */
    bool create(EGEIndexBuffer::IndexSize size, u32 count = 0) override;

    /* IndexBuffer override. Locks buffer's given part of the buffer for read/write operations. 
     * @param offset  0-based index offset from which locking should be done. 
     * @param count   Number of indicies to lock.
     */
    void* lock(u32 offset, u32 count) override;
    /* IndexBuffer override. Unlocks buffer. */
    void  unlock() override;

    /* IndexBuffer override. Returns number of allocated indicies. */
    u32 indexCount() const override;
 
  private:

    /* IndexBuffer override. Destroys buffer. */
    void destroy() override;
    /* Reallocates internal buffer to accomodate given number of indicies. */
    bool reallocateBuffer(u32 count);

  private:

    /*! Data buffser. */
    PDataBuffer m_buffer;         
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_INDEXBUFFERVAOGL_H