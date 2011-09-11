#include "Core/Data/DataBuffer.h"
#include "Core/Math/Math.h"
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(DataBuffer)
EGE_DEFINE_DELETE_OPERATORS(DataBuffer)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer::DataBuffer() : Object(NULL, EGE_OBJECT_UID_DATA_BUFFER), m_size(0), m_capacity(0), m_data(NULL), m_readOffset(0), m_writeOffset(0), m_mutable(true)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Wraps around given data pointer and size. Resulting buffer is not mutable */
DataBuffer::DataBuffer(void* data, s64 size) : Object(NULL, EGE_OBJECT_UID_DATA_BUFFER), m_size(size), m_capacity(size), m_data(data), m_readOffset(0), 
                                               m_writeOffset(0), m_mutable(false)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer::~DataBuffer()
{
  clear(true);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Gets pointer to data at given byte offset. */
void* DataBuffer::data(s64 offset) const
{
    return (offset < size()) ? (static_cast<u8*>(m_data) + offset) : NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Clears buffer. If FULL flag is set deallocates internal buffers as well. */
void DataBuffer::clear(bool full)
{
  // reset size
  m_size = 0;

  // reset offsets
  m_writeOffset = 0;
  m_readOffset = 0;

  // check if full clearing should be done and can be done
  if (full && m_mutable)
  {
    // deallocate buffer
    EGE_FREE(m_data);

    // reset capacity
    m_capacity = 0;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets buffer capacity in bytes. */
EGEResult DataBuffer::setCapacity(s64 newCapacity)
{
  // check if data cannot be changed
  if (!m_mutable)
  {
    return EGE_ERROR_NOT_SUPPORTED;
  }

  // check if new capacity greater than current one
  if (newCapacity > capacity())
  {
    // reallocate memory
    void* data = EGE_REALLOC(m_data, static_cast<size_t>(newCapacity));
    if (NULL == data)
    {
      // error!
      return EGE_ERROR_NO_MEMORY;
    }

    // store new pointer
    m_data = data;

    // set new capacity
    m_capacity = newCapacity;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Writes given data at provided offset to buffer. Returns number of data written. */
s64 DataBuffer::write(void* data, s64 size)
{
  EGE_ASSERT(0 <= size && data);

  // check if there is NO enough capacity for new data
  if (size + writeOffset() >= capacity())
  {
    // more space is required
    if (EGE_SUCCESS != setCapacity(size + writeOffset() + 1))
    {
      // error!
      return 0;
    }
  }

  // copy new data
  MemoryManager::MemCpy(static_cast<u8*>(m_data) + writeOffset(), data, static_cast<size_t>(size));

  // set new size
  // NOTE: taking MAX of what was before and after as we may want to overwrite data in the middle of buffer only
  m_size = Math::Max(m_size, size + writeOffset());

  // update write offset
  m_writeOffset += size;

  return size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Reads given amount of data from buffer. Returns number of data read. */
s64 DataBuffer::read(void* data, s64 size)
{
  EGE_ASSERT(0 <= size && data);

  // make sure only valid data is read
  size = Math::Min(this->size() - readOffset(), size);

  // copy new data
  MemoryManager::MemCpy(data, this->data(readOffset()), static_cast<size_t>(size));

  // update read offset
  m_readOffset += size;

  return size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets buffer size in bytes. */
EGEResult DataBuffer::setSize(s64 size)
{
  // check if there is NO enough capacity for new size
  if (size > capacity())
  {
    // more space is required
    EGEResult eResult = setCapacity(size);
    if (EGE_SUCCESS != eResult)
    {
      // error!
      return eResult;
    }
  }

  // store new size
  m_size = size;

  // TAGE - hmm what about read/write offsets ?

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets write offset. Does nothing if offset out of range [0, capacity]. */
/// @return Returns old write offset.
s64 DataBuffer::setWriteOffset(s64 offset)
{
  s64 oldOffset = writeOffset();

  if (offset <= capacity())
  {
    m_writeOffset = offset;
  }

  return oldOffset;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets read offset. Does nothing if offset out of range [0, size]. */
/// @return Returns old read offset.
s64 DataBuffer::setReadOffset(s64 offset)
{
  s64 oldOffset = readOffset();

  if (offset <= size())
  {
    m_readOffset = offset;
  }

  return oldOffset;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer& DataBuffer::operator << (u8 value)
{
  write(&value, sizeof (value));
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer& DataBuffer::operator << (s8 value)
{
  write(&value, sizeof (value));
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer& DataBuffer::operator << (u16 value)
{
  u8 data[2] = { value & 0x00ff, (value & 0xff00) >> 8 };

  write(&data, sizeof (value));
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer& DataBuffer::operator << (s16 value)
{
  u8 data[2] = { value & 0x00ff, (value & 0xff00) >> 8 };

  write(&data, sizeof (value));
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer& DataBuffer::operator << (u32 value)
{
  u8 data[4] = { (u8)(value & 0x000000ff), (u8)((value & 0x0000ff00) >> 8), (u8)((value & 0x00ff0000) >> 16), (u8)((value & 0xff000000) >> 24) };

  write(&data, sizeof (value));
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer& DataBuffer::operator << (s32 value)
{
  u8 data[4] = { (u8)(value & 0x000000ff), (u8)((value & 0x0000ff00) >> 8), (u8)((value & 0x00ff0000) >> 16), (u8)((value & 0xff000000) >> 24) };

  write(&data, sizeof (value));
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer& DataBuffer::operator << (u64 value)
{
  u8 data[8] = { (u8)((value & 0x00000000000000ffLL) >> 0), (u8)((value & 0x000000000000ff00LL) >> 8), 
                 (u8)((value & 0x0000000000ff0000LL) >> 16), (u8)((value & 0x00000000ff000000LL) >> 24),
                 (u8)((value & 0x000000ff00000000LL) >> 32), (u8)((value & 0x0000ff0000000000LL) >> 40), 
                 (u8)((value & 0x00ff000000000000LL) >> 48), (u8)((value & 0xff00000000000000LL) >> 56) };

  write(&data, sizeof (value));
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer& DataBuffer::operator << (s64 value)
{
  u8 data[8] = { (u8)((value & 0x00000000000000ffLL) >> 0), (u8)((value & 0x000000000000ff00LL) >> 8), 
                 (u8)((value & 0x0000000000ff0000LL) >> 16), (u8)((value & 0x00000000ff000000LL) >> 24),
                 (u8)((value & 0x000000ff00000000LL) >> 32), (u8)((value & 0x0000ff0000000000LL) >> 40), 
                 (u8)((value & 0x00ff000000000000LL) >> 48), (u8)((value & 0xff00000000000000LL) >> 56) };

  write(&data, sizeof (value));
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer& DataBuffer::operator << (bool value)
{
  return this->operator<< ((u8)(value ? 1 : 0));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer& DataBuffer::operator << (float32 value)
{
  write(&value, sizeof (value));
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer& DataBuffer::operator << (float64 value)
{
  write(&value, sizeof (value));
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer& DataBuffer::operator >> (u8& value)
{
  read(&value, sizeof (value));
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer& DataBuffer::operator >> (s8& value)
{
  read(&value, sizeof (value));
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer& DataBuffer::operator >> (u16& value)
{
  u8 data[2];
  if (sizeof (value) == read(data, sizeof (value)))
  {
    value = (u16)(data[0]) | ((u16)(data[1]) << 8);
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer& DataBuffer::operator >> (s16& value)
{
  u8 data[2];
  if (sizeof (value) == read(data, sizeof (value)))
  {
    value = (u16)(data[0]) | ((u16)(data[1]) << 8);
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer& DataBuffer::operator >> (u32& value)
{
  u8 data[4];
  if (sizeof (value) == read(data, sizeof (value)))
  {
    value = (u32)(data[0]) | ((u32)(data[1]) << 8) | ((u32)(data[2]) << 16) | ((u32)(data[3]) << 24);
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer& DataBuffer::operator >> (s32& value)
{
  u8 data[4];
  if (sizeof (value) == read(data, sizeof (value)))
  {
    value = (u32)(data[0]) | ((u32)(data[1]) << 8) | ((u32)(data[2]) << 16) | ((u32)(data[3]) << 24);
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer& DataBuffer::operator >> (u64& value)
{
  u8 data[8];
  if (sizeof (value) == read(data, sizeof (value)))
  {
    value = (u64)(data[0]) | ((u64)(data[1]) << 8) | ((u64)(data[2]) << 16) | ((u64)(data[3]) << 24) | ((u64)(data[4]) << 32) | ((u64)(data[5]) << 40) |
            ((u64)(data[6]) << 48) | ((u64)(data[7]) << 56);
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer& DataBuffer::operator >> (s64& value)
{
  u8 data[8];
  if (sizeof (value) == read(data, sizeof (value)))
  {
    value = (u64)(data[0]) | ((u64)(data[1]) << 8) | ((u64)(data[2]) << 16) | ((u64)(data[3]) << 24) | ((u64)(data[4]) << 32) | ((u64)(data[5]) << 40) |
            ((u64)(data[6]) << 48) | ((u64)(data[7]) << 56);
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer& DataBuffer::operator >> (bool& value)
{
  u8 data;
  this->operator>>(data);
  value = (1 == data);

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer& DataBuffer::operator >> (float32& value)
{
  read(&value, sizeof (value));
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer& DataBuffer::operator >> (float64& value)
{
  read(&value, sizeof (value));
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Reads given amount of data into destination buffer.
 * @note   Destination buffer write offset will be updated accordingly.
 * @return Returns number of bytes read.
 */
s64 DataBuffer::read(const PDataBuffer& dst, s64 size)
{
  EGE_ASSERT(0 <= size);

  // make sure only valid data is read
  size = Math::Min(this->size() - readOffset(), size);

  // copy new data
  // NOTE: dst buffer write offset will be updated automatically
  dst->write(data(readOffset()), size);

  // update read offset
  m_readOffset += size;

  return size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Writes given amount of data from destination buffer.
 * @note   Source buffer read offset will be updated accordingly.
 * @return Returns number of bytes written.
 */
s64 DataBuffer::write(const PDataBuffer& src, s64 size)
{
  EGE_ASSERT(0 <= size);

  // check if there is NO enough capacity for new data
  if (size + writeOffset() >= capacity())
  {
    // more space is required
    if (EGE_SUCCESS != setCapacity(size + writeOffset() + 1))
    {
      // error!
      return 0;
    }
  }

  // copy new data
  // NOTE: src buffer read offset will be updated automatically
  src->read(data(writeOffset()), size);
  //MemoryManager::MemCpy(static_cast<u8*>(m_data) + writeOffset(), src->data(src->readOffset()), static_cast<size_t>(size));

  // set new size
  // NOTE: taking MAX of what was before and after as we may want to overwrite data in the middle of buffer only
  m_size = Math::Max(m_size, size + writeOffset());

  // update write offset
  m_writeOffset += size;

  return size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
