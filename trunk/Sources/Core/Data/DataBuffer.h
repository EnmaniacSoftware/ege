#ifndef EGE_CORE_DATABUFFER_H
#define EGE_CORE_DATABUFFER_H

#include "EGE.h"
#include "EGEByteOrder.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(DataBuffer, PDataBuffer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class DataBuffer : public Object
{
  public:

    DataBuffer();
    /* Wraps around given data pointer and size. Resulting buffer is not mutable */
    DataBuffer(void* data, s64 size);
    virtual ~DataBuffer();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Sets buffer size in bytes. */
    EGEResult setSize(s64 size);
    /*! Gets buffer size in bytes. */
    s64 size() const { return m_size; }

    /*1 Sets buffer capacity in bytes. */
    EGEResult setCapacity(s64 capacity);
    /*! Gets buffer capacity in bytes. */
    s64 capacity() const { return m_capacity; }

    /*! Gets pointer to data at given byte offset. */
    void* data(s64 offset = 0) const;
    /*! Writes given amount of data into buffer. Returns number of data written. */
    s64 write(const void* data, s64 size);
    /*! Reads given amount of data from buffer. Returns number of data read. */
    s64 read(void* data, s64 size);
    /*! Reads given amount of data into destination buffer.
     *  @note   Destination buffer write offset will be updated accordingly.
     *  @return Returns number of bytes read.
     */
    s64 read(const PDataBuffer& dst, s64 size);
    /*! Writes given amount of data from destination buffer.
     *  @note   Source buffer read offset will be updated accordingly.
     *  @return Returns number of bytes written.
     */
    s64 write(const PDataBuffer& src, s64 size = -1);
    /*! Clears buffer. If FULL flag is set deallocates internal buffers as well. */
    void clear(bool full = false);
    /*! Sets write offset. Does nothing if offset out of range [0, capacity].
     *  @return Returns old write offset.
     */
    s64 setWriteOffset(s64 offset);
    /*! Returns write offset. */
    s64 writeOffset() const { return m_writeOffset; }
    /*! Sets read offset. Does nothing if offset out of range [0, size].
     *  @return Returns old read offset.
     */
    s64 setReadOffset(s64 offset);
    /*! Returns read offset. */
    s64 readOffset() const { return m_readOffset; }
    
    /*! Sets byte ordering. */
    void setByteOrdering(Endianness ordering);
    /*! Returns current byte ordering. */
    Endianness byteOrdering() const { return m_byteOrdering; }

    DataBuffer& operator << (u8 value);
    DataBuffer& operator << (s8 value);
    DataBuffer& operator << (u16 value);
    DataBuffer& operator << (s16 value);
    DataBuffer& operator << (u32 value);
    DataBuffer& operator << (s32 value);
    DataBuffer& operator << (u64 value);
    DataBuffer& operator << (s64 value);
    DataBuffer& operator << (bool value);
    DataBuffer& operator << (float32 value);
    DataBuffer& operator << (float64 value);
    DataBuffer& operator >> (u8& value);
    DataBuffer& operator >> (s8& value);
    DataBuffer& operator >> (u16& value);
    DataBuffer& operator >> (s16& value);
    DataBuffer& operator >> (u32& value);
    DataBuffer& operator >> (s32& value);
    DataBuffer& operator >> (u64& value);
    DataBuffer& operator >> (s64& value);
    DataBuffer& operator >> (bool& value);
    DataBuffer& operator >> (float32& value);
    DataBuffer& operator >> (float64& value);

  private:

    /*! Current size (used) amount (in bytes). */
    s64 m_size;
    /*! Max size possible without reallocation (in bytes). */
    s64 m_capacity;
    /*! Pointer to allocated data. */
    void* m_data;
    /*! Read pointer offset. */
    s64 m_readOffset;
    /*! Write pointer offset. */
    s64 m_writeOffset;
    /*! TRUE if internal data can be changed, ie. realloc, free etc. */
    bool m_mutable;
    /*! Current byte ordering. */
    Endianness m_byteOrdering;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DATABUFFER_H