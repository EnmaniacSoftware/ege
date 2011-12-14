#ifndef EGE_CORE_INDEXBUFFER_H
#define EGE_CORE_INDEXBUFFER_H

#include <EGE.h>
#include <EGEIndexBuffer.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(IndexBuffer, PIndexBuffer)
EGE_DECLARE_SMART_CLASS(DataBuffer, PDataBuffer)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class IndexBuffer : public Object
{
  public:

    IndexBuffer(Application* app);
   ~IndexBuffer();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Creates buffer for requested number of indicies of given size. */
    bool create(EGEIndexBuffer::IndexSize size, u32 count = 0);
    /* Destroys buffer. */
    void destroy();
    /* Locks buffer given part of the buffer for read/write operations. */
    void* lock(u32 offset, u32 count);
    /* Unlocks buffer. */
    void  unlock();
    /* Returns number of allocated indicies. */
    u32 indexCount() const;
    /* Returns index size (in bytes). */
    u8 indexSize() const;
    /*! Returns size type. */
    inline EGEIndexBuffer::IndexSize size() const { return m_size; }
 
  private:

    /* Reallocates internal buffer to accomodate given number of indicies. */
    bool reallocateBuffer(u32 count);

  private:

    /*! Size of indicies. */
    EGEIndexBuffer::IndexSize m_size;
    /*! TRUE if buffer is locked. */
    bool m_locked;
    /*! Data buffser. */
    PDataBuffer m_buffer;         
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_INDEXBUFFER_H