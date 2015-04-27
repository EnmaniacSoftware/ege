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
DataBuffer::DataBuffer() 
: Object(EGE_OBJECT_UID_DATA_BUFFER)
, m_size(0)
, m_capacity(0)
, m_data(NULL)
, m_readOffset(0)
, m_writeOffset(0)
, m_mutable(true)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer::DataBuffer(const DataBuffer& other) 
: Object(EGE_OBJECT_UID_DATA_BUFFER)
, m_size(0)
, m_capacity(0)
, m_data(NULL)
, m_readOffset(0)
, m_writeOffset(0)
, m_mutable(true)
{
  // allocate space for data
  if (EGE_SUCCESS == setSize(other.size()))
  {
    // copy data
    EGE_MEMCPY(m_data, other.m_data, static_cast<size_t>(other.size()));

    // copy rest of data
    m_readOffset  = other.readOffset();
    m_writeOffset = other.writeOffset();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer::DataBuffer(s64 size) 
: Object(EGE_OBJECT_UID_DATA_BUFFER)
, m_size(0)
, m_capacity(0)
, m_data(NULL)
, m_readOffset(0)
, m_writeOffset(0)
, m_mutable(true)
{
  if (EGE_SUCCESS != setSize(size))
  {
    // error!
    egeWarning(KDataBufferDebugName) << "Could not set data buffer size to:" << size;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataBuffer::DataBuffer(const void* data, s64 size) 
: Object(EGE_OBJECT_UID_DATA_BUFFER)
, m_size(size)
, m_capacity(size)
, m_data(const_cast<void*>(data))
, m_readOffset(0)
, m_writeOffset(0)
, m_mutable(false)
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
  return ((offset < capacity()) && (0 <= offset)) ? (static_cast<u8*>(m_data) + offset) : NULL;
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
  if ( ! m_mutable)
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

  if ( ! m_mutable)
  {
    // error!
    return -1;
  }

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
  // check if data cannot be changed
  if ( ! m_mutable)
  {
    return EGE_ERROR_NOT_SUPPORTED;
  }

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
s64 DataBuffer::read(const PDataBuffer& destination, s64 size)
{
  EGE_ASSERT(0 <= size);

  // make sure only valid data is read
  size = Math::Min(this->size() - readOffset(), size);

  // copy new data
  // NOTE: dst buffer write offset will be updated automatically
  const s64 written = destination->write(data(readOffset()), size);
  if (0 < written)
  {
    // update read offset
    m_readOffset += written;
  }

  return written;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 DataBuffer::write(const PDataBuffer& source, s64 size)
{
  if ( ! m_mutable)
  {
    // error!
    return -1;
  }

  // check if all data from source buffer requested
  if (0 > size)
  {
    size = source->size() - source->readOffset();
  }

  // check if there is NO enough capacity for new data
  if (size + writeOffset() >= capacity())
  {
    // more space is required
    if (EGE_SUCCESS != setCapacity(size + writeOffset() + 1))
    {
      // error!
      return -1;
    }
  }

  // copy new data
  // NOTE: src buffer read offset will be updated automatically
  const s64 bytesRead = source->read(data(writeOffset()), size);

  if (0 < bytesRead)
  {
    // set new size
    // NOTE: taking MAX of what was before and after as we may want to overwrite data in the middle of buffer only
    m_size = Math::Max(m_size, writeOffset() + bytesRead);

    // update write offset
    m_writeOffset += bytesRead;
  }

  return bytesRead;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 DataBuffer::writeOffset() const 
{ 
  return m_writeOffset; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 DataBuffer::readOffset() const 
{ 
  return m_readOffset; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 DataBuffer::size() const 
{ 
  return m_size; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 DataBuffer::capacity() const 
{ 
  return m_capacity; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END