#ifndef EGE_CORE_INDEXBUFFER_H
#define EGE_CORE_INDEXBUFFER_H

#include "EGE.h"

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

    enum Size
    {
      SIZE_UNKNOWN = -1,
      SIZE_8BIT,
      SIZE_16BIT,
      SIZE_32BIT
    };

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Creates buffer for requested number of indicies of given size. */
    bool create(Size size, u32 count);
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
 
  private:

    /* Reallocates internal buffer to accomodate given number of indicies. */
    bool reallocateBuffer(u32 count);

  private:

    /*! Size of indicies. */
    Size m_size;
    /*! TRUE if buffer is locked. */
    bool m_locked;
    /*! Data buffser. */
    PDataBuffer m_buffer;         
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_INDEXBUFFER_H