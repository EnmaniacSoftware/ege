#include "Core/Data/Interface/DataStream.h"
#include "Core/Math/Interface/Math.h"
#include "EGEDebug.h"
#include "EGEIODevice.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//EGE_DEFINE_NEW_OPERATORS(DataStream)
//EGE_DEFINE_DELETE_OPERATORS(DataStream)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataStream::DataStream(IODevice* device, Endianness byteOrdering) 
: m_device(device)
, m_byteOrdering(byteOrdering)
, m_good(true)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataStream::~DataStream()
{
  m_device = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DataStream::setByteOrdering(Endianness ordering)
{
  m_byteOrdering = ordering;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Endianness DataStream::byteOrdering() const 
{ 
  return m_byteOrdering; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataStream& DataStream::operator << (u8 value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    // write to device
    const s64 written = m_device->write(&value, sizeof (value));

    // update state
    m_good = (sizeof (value) == written);
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataStream& DataStream::operator << (s8 value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    // write to device
    const s64 written = m_device->write(&value, sizeof (value));

    // update state
    m_good = (sizeof (value) == written);
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataStream& DataStream::operator << (u16 value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    u8 data[2];
    ByteOrder::Convert(data, value, byteOrdering());

    // write to device
    const s64 written = m_device->write(&data, sizeof (value));

    // update state
    m_good = (sizeof (value) == written);
 }
  
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataStream& DataStream::operator << (s16 value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    u8 data[2];
    ByteOrder::Convert(data, value, byteOrdering());

    // write to device
    const s64 written = m_device->write(&data, sizeof (value));

    // update state
    m_good = (sizeof (value) == written);
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataStream& DataStream::operator << (u32 value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    u8 data[4];
    ByteOrder::Convert(data, value, byteOrdering());

    // write to device
    const s64 written = m_device->write(&data, sizeof (value));

    // update state
    m_good = (sizeof (value) == written);
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataStream& DataStream::operator << (s32 value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    u8 data[4];
    ByteOrder::Convert(data, value, byteOrdering());

    // write to device
    const s64 written = m_device->write(&data, sizeof (value));

    // update state
    m_good = (sizeof (value) == written);
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataStream& DataStream::operator << (u64 value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    u8 data[8];
    ByteOrder::Convert(data, value, byteOrdering());

    // write to device
    const s64 written = m_device->write(&data, sizeof (value));

    // update state
    m_good = (sizeof (value) == written);
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataStream& DataStream::operator << (s64 value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    u8 data[8];
    ByteOrder::Convert(data, value, byteOrdering());

    // write to device
    const s64 written = m_device->write(&data, sizeof (value));

    // update state
    m_good = (sizeof (value) == written);
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataStream& DataStream::operator << (bool value)
{
  return this->operator<< ((u8)(value ? 1 : 0));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataStream& DataStream::operator << (float32 value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    // write to device
    const s64 written = m_device->write(&value, sizeof (value));

    // update state
    m_good = (sizeof (value) == written);
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataStream& DataStream::operator << (float64 value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    // write to device
    const s64 written = m_device->write(&value, sizeof (value));

    // update state
    m_good = (sizeof (value) == written);
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataStream& DataStream::operator << (const char* value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    const s32 length = static_cast<s32>(strlen(value) + 1);

    // NOTE: include NULL terminator
    const s64 written = m_device->write(value, length);

    // update state
    m_good = (length == written);
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataStream& DataStream::operator >> (u8& value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    // read from device
    const s64 read = m_device->read(&value, sizeof (value));

    // update state
    m_good = (sizeof (value) == read);
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataStream& DataStream::operator >> (s8& value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    // read from device
    const s64 read = m_device->read(&value, sizeof (value));

    // update state
    m_good = (sizeof (value) == read);
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataStream& DataStream::operator >> (u16& value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    u8 data[2];

    // read from device
    const s64 read = m_device->read(data, sizeof (value));

    // convert endianess
    ByteOrder::Convert(value, data, byteOrdering());

    // update state
    m_good = (sizeof (value) == read);
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataStream& DataStream::operator >> (s16& value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    u8 data[2];

    // read from device
    const s64 read = m_device->read(data, sizeof (value));

    // convert endianess
    ByteOrder::Convert(value, data, byteOrdering());

    // update state
    m_good = (sizeof (value) == read);
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataStream& DataStream::operator >> (u32& value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    u8 data[4];

    // read from device
    const s64 read = m_device->read(data, sizeof (value));

    // convert endianess
    ByteOrder::Convert(value, data, byteOrdering());

    // update state
    m_good = (sizeof (value) == read);
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataStream& DataStream::operator >> (s32& value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    u8 data[4];

    // read from device
    const s64 read = m_device->read(data, sizeof (value));

    // convert endianess
    ByteOrder::Convert(value, data, byteOrdering());

    // update state
    m_good = (sizeof (value) == read);
  }
  
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataStream& DataStream::operator >> (u64& value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    u8 data[8];

    // read from device
    const s64 read = m_device->read(data, sizeof (value));

    // convert endianess
    ByteOrder::Convert(value, data, byteOrdering());

    // update state
    m_good = (sizeof (value) == read);
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataStream& DataStream::operator >> (s64& value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    u8 data[8];

    // read from device
    const s64 read = m_device->read(data, sizeof (value));

    // convert endianess
    ByteOrder::Convert(value, data, byteOrdering());

    // update state
    m_good = (sizeof (value) == read);
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataStream& DataStream::operator >> (bool& value)
{
  EGE_ASSERT(NULL != m_device);

  u8 data = 0;
  
  // read from device
  this->operator>>(data);
  
  // convert result
  value = (1 == data);

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataStream& DataStream::operator >> (float32& value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    // read from device
    const s64 read = m_device->read(&value, sizeof (value));

    // update state
    m_good = (sizeof (value) == read);
  }
  
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataStream& DataStream::operator >> (float64& value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    // read from device
    const s64 read = m_device->read(&value, sizeof (value));

    // update state
    m_good = (sizeof (value) == read);
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DataStream& DataStream::operator >> (char* value)
{
  EGE_ASSERT(NULL != m_device);

  // read until NULL terminator is found
  while (isGood())
  {
    m_good = (1 == m_device->read(value, 1));
    
    if ('\0' == *value)
    {
      // done
      break;
    }

    ++value;
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 DataStream::write(const void* data, s64 length)
{
  EGE_ASSERT(NULL != m_device);

  s64 written = -1;

  if (isGood())
  {
    written = m_device->write(data, length);

    // update state
    m_good = (written == length);
  }

  return written;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 DataStream::read(void* data, s64 length)
{
  EGE_ASSERT(NULL != m_device);

  s64 read = -1;

  if (isGood())
  {
    read = m_device->read(data, length);

    // update state
    m_good = (read == length);
  }

  return read;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool DataStream::isGood() const
{
  return m_good;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END