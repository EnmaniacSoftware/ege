#include "Core/Data/Interface/DataBuffer.h"
#include "Core/Math/Interface/Math.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KDataBufferDebugName = "EGEDataBuffer";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(DataBuffer)
EGE_DEFINE_DELETE_OPERATORS(DataBuffer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer::DataBuffer() : Object(NULL, EGE_OBJECT_UID_DATA_BUFFER), 
                           m_size(0), 
                           m_capacity(0), 
                           m_data(NULL), 
                           m_readOffset(0), 
                           m_writeOffset(0), 
                           m_mutable(true),
                           m_byteOrdering(ELittleEndian)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer::DataBuffer(const DataBuffer& other) : Object(NULL, EGE_OBJECT_UID_DATA_BUFFER),
                                                  m_size(0), 
                                                  m_capacity(0), 
                                                  m_data(NULL), 
                                                  m_readOffset(0), 
                                                  m_writeOffset(0), 
                                                  m_mutable(true),
                                                  m_byteOrdering(ELittleEndian)
{
  // allocate space for data
  if (EGE_SUCCESS == setSize(other.size()))
  {
    // copy data
    EGE_MEMCPY(m_data, other.m_data, static_cast<size_t>(other.size()));

    // copy rest of data
    m_readOffset    = other.readOffset();
    m_writeOffset   = other.writeOffset();
    m_byteOrdering  = other.byteOrdering();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer::DataBuffer(s64 size) : Object(NULL, EGE_OBJECT_UID_DATA_BUFFER)
                                 , m_size(0)
                                 , m_capacity(0)
                                 , m_data(NULL)
                                 , m_readOffset(0)
                                 , m_writeOffset(0)
                                 , m_mutable(true)
                                 , m_byteOrdering(ELittleEndian)
{
  if (EGE_SUCCESS != setSize(size))
  {
    // error!
    egeWarning(KDataBufferDebugName) << "Could not set data buffer size to:" << size;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer::DataBuffer(const void* data, s64 size) : Object(NULL, EGE_OBJECT_UID_DATA_BUFFER), 
                                                     m_size(size), 
                                                     m_capacity(size), 
                                                     m_data(const_cast<void*>(data)), 
                                                     m_readOffset(0), 
                                                     m_writeOffset(0), 
                                                     m_mutable(false),
                                                     m_byteOrdering(ELittleEndian)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer::~DataBuffer()
{
  clear(true);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void* DataBuffer::data(s64 offset) const
{
  return (offset < capacity()) ? (static_cast<u8*>(m_data) + offset) : NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
s64 DataBuffer::write(const void* data, s64 size)
{
  EGE_ASSERT(0 <= size && data);

  // check if there is NO enough capacity for new data
  if (size + writeOffset() > capacity())
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
void DataBuffer::setByteOrdering(Endianness ordering)
{
  m_byteOrdering = ordering;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& DataBuffer::operator << (u8 value)
{
  write(&value, sizeof (value));
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& DataBuffer::operator << (s8 value)
{
  write(&value, sizeof (value));
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& DataBuffer::operator << (u16 value)
{
  u8 data[2];
  ByteOrder::Convert(data, value, byteOrdering());

  write(&data, sizeof (value));
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& DataBuffer::operator << (s16 value)
{
  u8 data[2];
  ByteOrder::Convert(data, value, byteOrdering());

  write(&data, sizeof (value));
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& DataBuffer::operator << (u32 value)
{
  u8 data[4];
  ByteOrder::Convert(data, value, byteOrdering());

  write(&data, sizeof (value));
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& DataBuffer::operator << (s32 value)
{
  u8 data[4];
  ByteOrder::Convert(data, value, byteOrdering());

  write(&data, sizeof (value));
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& DataBuffer::operator << (u64 value)
{
  u8 data[8];
  ByteOrder::Convert(data, value, byteOrdering());

  write(&data, sizeof (value));
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& DataBuffer::operator << (s64 value)
{
  u8 data[8];
  ByteOrder::Convert(data, value, byteOrdering());

  write(&data, sizeof (value));
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& DataBuffer::operator << (bool value)
{
  return this->operator<< ((u8)(value ? 1 : 0));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& DataBuffer::operator << (float32 value)
{
  write(&value, sizeof (value));
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& DataBuffer::operator << (float64 value)
{
  write(&value, sizeof (value));
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& DataBuffer::operator << (const char* value)
{
  EGE_UNUSED(value);
  
  EGE_ASSERT_X(false, "Unsupported");
  
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& DataBuffer::operator << (const String& value)
{
  EGE_UNUSED(value);

  EGE_ASSERT_X(false, "Unsupported");
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& DataBuffer::operator >> (u8& value)
{
  read(&value, sizeof (value));
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& DataBuffer::operator >> (s8& value)
{
  read(&value, sizeof (value));
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& DataBuffer::operator >> (u16& value)
{
  u8 data[2];
  if (sizeof (value) == read(data, sizeof (value)))
  {
    ByteOrder::Convert(value, data, byteOrdering());
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& DataBuffer::operator >> (s16& value)
{
  u8 data[2];
  if (sizeof (value) == read(data, sizeof (value)))
  {
    ByteOrder::Convert(value, data, byteOrdering());
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& DataBuffer::operator >> (u32& value)
{
  u8 data[4];
  if (sizeof (value) == read(data, sizeof (value)))
  {
    ByteOrder::Convert(value, data, byteOrdering());
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& DataBuffer::operator >> (s32& value)
{
  u8 data[4];
  if (sizeof (value) == read(data, sizeof (value)))
  {
    ByteOrder::Convert(value, data, byteOrdering());
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& DataBuffer::operator >> (u64& value)
{
  u8 data[8];
  if (sizeof (value) == read(data, sizeof (value)))
  {
    ByteOrder::Convert(value, data, byteOrdering());
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& DataBuffer::operator >> (s64& value)
{
  u8 data[8];
  if (sizeof (value) == read(data, sizeof (value)))
  {
    ByteOrder::Convert(value, data, byteOrdering());
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& DataBuffer::operator >> (bool& value)
{
  u8 data;
  this->operator>>(data);
  value = (1 == data);

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& DataBuffer::operator >> (float32& value)
{
  read(&value, sizeof (value));
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& DataBuffer::operator >> (float64& value)
{
  read(&value, sizeof (value));
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& DataBuffer::operator >> (const char* value)
{
  EGE_UNUSED(value);
  
  EGE_ASSERT_X(false, "Unsupported");
  
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& DataBuffer::operator >> (String& value)
{
  EGE_UNUSED(value);
  
  EGE_ASSERT_X(false, "Unsupported");
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
s64 DataBuffer::write(const PDataBuffer& src, s64 size)
{
  // check if all data from source buffer requested
  if (0 > size)
  {
    size = src->size() - src->readOffset();
  }

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

  // set new size
  // NOTE: taking MAX of what was before and after as we may want to overwrite data in the middle of buffer only
  m_size = Math::Max(m_size, size + writeOffset());

  // update write offset
  m_writeOffset += size;

  return size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END