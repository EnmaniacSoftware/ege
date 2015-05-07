#include "Core/String/Interface/TextStream.h"
#include "EGEDebug.h"
#include "EGEIODevice.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsWhiteSpace(char c)
{
  return (' ' == c) || ('\t' == c) || ('\n' == c) || ('\r' == c) || ('\v' == c) || ('\f' == c);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function reading next word from a device.
 *  @param  device      Device to the data from.
 *  @param  buffer      Buffer to store data to.
 *  @param  bufferSize  Size of the buffer.
 *  @return Number of bytes stored in the buffer upto maximum of buffer size.
 *  @note This function consumes any whitespaces before the read content.
 */
static s32 ReadWord(IODevice& device, char* buffer, s32 bufferSize)
{
  EGE_ASSERT(1 < bufferSize);

  s32 bytesRead = 0;

  bool done = false;
  do
  {
    // try to read a character
    char c;
    s64 read = device.read(&c, 1);

    // check if no more data in the device
    done = (1 != read);

    // check if whitespace
    if ( ! done && IsWhiteSpace(c))
    {
      // has anything been read already
      if (0 < bytesRead)
      {
        // done
        done = true;
      }
    }
    else if ( ! done && (bytesRead < bufferSize))
    {
      // store read character
      buffer[bytesRead] = c;
      ++bytesRead;
    }

  } while ( ! done);

  return bytesRead;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function reading next word from a device.
 *  @param  device      Device to the data from.
 *  @param  buffer      String buffer to store data in.
 *  @return Number of bytes stored in the buffer.
 *  @note This function consumes any whitespaces before the read content.
 */
static s32 ReadWord(IODevice& device, String& buffer)
{
  s32 bytesRead = 0;

  bool done = false;
  do
  {
    // try to read a character
    char c;
    s64 read = device.read(&c, 1);

    // check if no more data in the device
    done = (1 != read);

    // check if whitespace
    if ( ! done && IsWhiteSpace(c))
    {
      // has anything been read already
      if (0 < bytesRead)
      {
        // done
        done = true;
      }
    }
    else if ( ! done)
    {
      // store read character
      buffer += c;
      ++bytesRead;
    }

  } while ( ! done);

  return bytesRead;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextStream::TextStream(IODevice* device) 
: m_device(device)
, m_good((NULL != device))
, m_floatPrecision(6)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextStream::~TextStream()
{
  m_device = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextStream& TextStream::operator << (u8 value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    const String& valueString = String::Format("%u", value);

    const s64 written = m_device->write(valueString.toAscii(), valueString.length());
    
    // update status
    m_good = (written == valueString.length());
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextStream& TextStream::operator << (s8 value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    const String& valueString = String::Format("%d", value);

    const s64 written = m_device->write(valueString.toAscii(), valueString.length());
    
    // update status
    m_good = (written == valueString.length());
  }
  
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextStream& TextStream::operator << (u16 value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    const String& valueString = String::Format("%u", value);

    const s64 written = m_device->write(valueString.toAscii(), valueString.length());
    
    // update status
    m_good = (written == valueString.length());
  }
  
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextStream& TextStream::operator << (s16 value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    const String& valueString = String::Format("%d", value);
  
    const s64 written = m_device->write(valueString.toAscii(), valueString.length());
    
    // update status
    m_good = (written == valueString.length());
  }
  
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextStream& TextStream::operator << (u32 value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    const String& valueString = String::Format("%u", value);

    const s64 written = m_device->write(valueString.toAscii(), valueString.length());
    
    // update status
    m_good = (written == valueString.length());
  }
  
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextStream& TextStream::operator << (s32 value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    const String& valueString = String::Format("%d", value);

    const s64 written = m_device->write(valueString.toAscii(), valueString.length());
    
    // update status
    m_good = (written == valueString.length());
  }
  
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextStream& TextStream::operator << (u64 value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    const String& valueString = String::Format("%llu", value);

    const s64 written = m_device->write(valueString.toAscii(), valueString.length());
    
    // update status
    m_good = (written == valueString.length());
  }
  
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextStream& TextStream::operator << (s64 value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    const String& valueString = String::Format("%lld", value);

    const s64 written = m_device->write(valueString.toAscii(), valueString.length());
    
    // update status
    m_good = (written == valueString.length());
  }
  
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextStream& TextStream::operator << (bool value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    const String& valueString = String::Format("%s", value ? "true" : "false");

    const s64 written = m_device->write(valueString.toAscii(), valueString.length());
    
    // update status
    m_good = (written == valueString.length());
  }
  
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextStream& TextStream::operator << (float32 value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    const String formatString = "%." + String::Format("%df", floatPrecision());
    const String valueString = String::Format(formatString.toAscii(), value);

    const s64 written = m_device->write(valueString.toAscii(), valueString.length());
    
    // update status
    m_good = (written == valueString.length());
  }
  
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextStream& TextStream::operator << (float64 value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    const String formatString = "%." + String::Format("%df", floatPrecision());
    const String valueString = String::Format(formatString.toAscii(), value);

    const s64 written = m_device->write(valueString.toAscii(), valueString.length());
    
    // update status
    m_good = (written == valueString.length());
  }
  
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextStream& TextStream::operator << (const char* value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    const String& valueString = value;

    const s64 written = m_device->write(valueString.toAscii(), valueString.length());
    
    // update status
    m_good = (written == valueString.length());
  }
  
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextStream& TextStream::operator << (const String& value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    const String& valueString = value;

    const s64 written = m_device->write(valueString.toAscii(), valueString.length());
    
    // update status
    m_good = (written == valueString.length());
  }
  
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextStream& TextStream::operator >> (u8& value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    const s32 KBufferSize = 4;
    char buffer[KBufferSize];

    // read a word
    const s32 bytesRead = ReadWord(*m_device, buffer, KBufferSize);

    // check if all ok so far
    if (0 < bytesRead)
    {
      // convert
      String string(buffer, bytesRead);
      value = static_cast<u8>(string.toInt(&m_good));
    }
    else
    {
      // nothing could be read
      m_good = false;
    }
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextStream& TextStream::operator >> (s8& value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    const s32 KBufferSize = 4;
    char buffer[KBufferSize];

    // read a word
    const s32 bytesRead = ReadWord(*m_device, buffer, KBufferSize);

    // check if all ok so far
    if (0 < bytesRead)
    {
      // convert
      String string(buffer, bytesRead);
      value = static_cast<s8>(string.toInt(&m_good));
    }
    else
    {
      // nothing could be read
      m_good = false;
    }
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextStream& TextStream::operator >> (u16& value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    const s32 KBufferSize = 8;
    char buffer[KBufferSize];

    // read a word
    const s32 bytesRead = ReadWord(*m_device, buffer, KBufferSize);

    // check if all ok so far
    if (0 < bytesRead)
    {
      // convert
      String string(buffer, bytesRead);
      value = static_cast<u16>(string.toInt(&m_good));
    }
    else
    {
      // nothing could be read
      m_good = false;
    }
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextStream& TextStream::operator >> (s16& value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    const s32 KBufferSize = 8;
    char buffer[KBufferSize];

    // read a word
    const s32 bytesRead = ReadWord(*m_device, buffer, KBufferSize);

    // check if all ok so far
    if (0 < bytesRead)
    {
      // convert
      String string(buffer, bytesRead);
      value = static_cast<s16>(string.toInt(&m_good));
    }
    else
    {
      // nothing could be read
      m_good = false;
    }
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextStream& TextStream::operator >> (u32& value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    const s32 KBufferSize = 16;
    char buffer[KBufferSize];

    // read a word
    const s32 bytesRead = ReadWord(*m_device, buffer, KBufferSize);

    // check if all ok so far
    if (0 < bytesRead)
    {
      // convert
      String string(buffer, bytesRead);
      value = string.toUInt(&m_good);
    }
    else
    {
      // nothing could be read
      m_good = false;
    }
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextStream& TextStream::operator >> (s32& value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    const s32 KBufferSize = 16;
    char buffer[KBufferSize];

    // read a word
    const s32 bytesRead = ReadWord(*m_device, buffer, KBufferSize);

    // check if all ok so far
    if (0 < bytesRead)
    {
      // convert
      String string(buffer, bytesRead);
      value = string.toInt(&m_good);
    }
    else
    {
      // nothing could be read
      m_good = false;
    }
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextStream& TextStream::operator >> (u64& value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    const s32 KBufferSize = 32;
    char buffer[KBufferSize];

    // read a word
    const s32 bytesRead = ReadWord(*m_device, buffer, KBufferSize);

    // check if all ok so far
    if (0 < bytesRead)
    {
      // convert
      String string(buffer, bytesRead);
      value = string.toUInt64(&m_good);
    }
    else
    {
      // nothing could be read
      m_good = false;
    }
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextStream& TextStream::operator >> (s64& value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    const s32 KBufferSize = 32;
    char buffer[KBufferSize];

    // read a word
    const s32 bytesRead = ReadWord(*m_device, buffer, KBufferSize);

    // check if all ok so far
    if (0 < bytesRead)
    {
      // convert
      String string(buffer, bytesRead);
      value = string.toInt64(&m_good);
    }
    else
    {
      // nothing could be read
      m_good = false;
    }
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextStream& TextStream::operator >> (bool& value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    const s32 KBufferSize = 8;
    char buffer[KBufferSize];

    // read a word
    const s32 bytesRead = ReadWord(*m_device, buffer, KBufferSize);

    // check if all ok so far
    if (0 < bytesRead)
    {
      // convert
      String string(buffer, bytesRead);
      value = string.toBool(&m_good);
    }
    else
    {
      // nothing could be read
      m_good = false;
    }
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextStream& TextStream::operator >> (float32& value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    const s32 KBufferSize = 32;
    char buffer[KBufferSize];

    // read a word
    const s32 bytesRead = ReadWord(*m_device, buffer, KBufferSize);

    // check if all ok so far
    if (0 < bytesRead)
    {
      // convert
      String string(buffer, bytesRead);
      value = string.toFloat(&m_good);
    }
    else
    {
      // nothing could be read
      m_good = false;
    }
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextStream& TextStream::operator >> (float64& value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    const s32 KBufferSize = 32;
    char buffer[KBufferSize];

    // read a word
    const s32 bytesRead = ReadWord(*m_device, buffer, KBufferSize);

    // check if all ok so far
    if (0 < bytesRead)
    {
      // convert
      String string(buffer, bytesRead);
      value = string.toFloat64(&m_good);
    }
    else
    {
      // nothing could be read
      m_good = false;
    }
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextStream& TextStream::operator >> (char* value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    String buffer;

    // read a word
    const s32 bytesRead = ReadWord(*m_device, buffer);

    // check if all ok so far
    if (0 < bytesRead)
    {
      // copy, including terminating NULL character
      EGE_MEMCPY(value, buffer.toAscii(), buffer.length() + 1);
    }
    else
    {
      // nothing could be read
      m_good = false;
    }
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextStream& TextStream::operator >> (String& value)
{
  EGE_ASSERT(NULL != m_device);

  if (isGood())
  {
    // read a word
    const s32 bytesRead = ReadWord(*m_device, value);

    // check if error
    if (0 >= bytesRead)
    {
      // nothing could be read
      m_good = false;
    }
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool TextStream::isGood() const
{
  return m_good;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TextStream::setFloatPrecision(s32 precision)
{
  EGE_ASSERT(0 <= precision);

  if (0 <= precision)
  {
    m_floatPrecision = precision;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 TextStream::floatPrecision() const
{
  return m_floatPrecision;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END