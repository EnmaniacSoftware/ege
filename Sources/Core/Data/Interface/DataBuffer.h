#ifndef EGE_CORE_DATA_DATABUFFER_H
#define EGE_CORE_DATA_DATABUFFER_H

/*! Convinient buffer for binary data storage. Allows writing and reading and acts as an I/O device for streaming purposes.
 */

#include "EGE.h"
#include "EGEIODevice.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_PREDECLARE_SMART_CLASS(DataBuffer, PDataBuffer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class DataBuffer : public Object
                 , public IODevice
{
  public:

    DataBuffer();
    DataBuffer(const DataBuffer& other);
    explicit DataBuffer(s64 size);
    /*! Wraps around given data pointer and size. Resulting buffer is not mutable */
    DataBuffer(const void* data, s64 size);
    virtual ~DataBuffer();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Resizes the buffer. 
     *  @param  size  Requested size in bytes.
     *  @return EGE_SUCCESS if operation succeeded. EGE_ERROR_NOT_SUPPORTED when buffer is not mutable. Otherwise, one of the available error codes.
     *  @note If a current capacity is smaller than requested size, it is increased to match the size.
     */
    EGEResult setSize(s64 size);
    /*! Returns buffer size in bytes. */
    s64 size() const;

    /*! Sets buffer capacity. 
     *  @param  capacity  Requested capacity in bytes. If new capacity is smaller than current one, no action is taken.
     *  @return EGE_SUCCESS if operation succeeded. EGE_ERROR_NOT_SUPPORTED when buffer is not mutable. Otherwise, one of the available error codes.
     */
    EGEResult setCapacity(s64 capacity);
    /*! Returns buffer capacity in bytes. */
    s64 capacity() const;

    /*! Returns pointer to the data at a given offset. 
     *  @param  offset  Offset in bytes from the begining of the data buffer.
     *  @return Pointer within the buffer or NULL if requested offset lies outside of the current capacity.
     */
    void* data(s64 offset = 0) const;

    /*! Reads given amount of data into destination buffer.
     *  @param  destination Destination buffer where data is to be stored.
     *  @param  size        Number of bytes to store in desitnation buffer.
     *  @return Returns number of bytes read. Negative value is returned in case of an error.
     *  @note Destination buffer write offset will be updated accordingly. Destination buffer size will be adjusted to fit all read data.
     */
    s64 read(const PDataBuffer& destination, s64 size);
    /*! Writes given amount of data from destination buffer.
     *  @param  source  Source buffer data is to be read from.
     *  @param  size    Number of bytes to read from the source buffer. Can be negative if all data is to be read.
     *  @return Returns number of bytes written. Negative value is returned in case of an error.
     *  @note   Source buffer read offset will be updated accordingly.
     */
    s64 write(const PDataBuffer& source, s64 size = -1);

    /*! @see IODevice::read. */
    s64 read(void* data, s64 length) override;
    /*! @see IODevice::write. */
    s64 write(const void* data, s64 length) override;

    /*! Clears buffer. If FULL flag is set deallocates internal buffers as well. */
    void clear(bool full = false);

    /*! Sets write offset. Does nothing if offset out of range [0, capacity].
     *  @return Returns old write offset.
     */
    s64 setWriteOffset(s64 offset);
    /*! Returns write offset. */
    s64 writeOffset() const;
    
    /*! Sets read offset. Does nothing if offset out of range [0, size].
     *  @return Returns old read offset.
     */
    s64 setReadOffset(s64 offset);
    /*! Returns read offset. */
    s64 readOffset() const;

  private:

    /*! Current size (used) amount (in bytes). */
    s64 m_size;
    /*! Max size possible without reallocation (in bytes). */
    s64 m_capacity;
    /*! Pointer to allocated data. */
    void* m_data;
    /*! Read pointer offset (0-based). */
    s64 m_readOffset;
    /*! Write pointer offset (0-based). */
    s64 m_writeOffset;
    /*! TRUE if internal data can be changed, ie. realloc, free etc. */
    bool m_mutable;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DATA_DATABUFFER_H