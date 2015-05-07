#include "Core/String/Interface/StringBuffer.h"
#include "EGEDebug.h"
#include "EGEMath.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(StringBuffer)
EGE_DEFINE_DELETE_OPERATORS(StringBuffer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
StringBuffer::StringBuffer() 
: Object(EGE_OBJECT_UID_STRING_BUFFER)
, m_readOffset(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
StringBuffer::~StringBuffer()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& StringBuffer::string() const
{
  return m_buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 StringBuffer::size() const
{
  return m_buffer.length();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 StringBuffer::read(void* data, s64 length)
{
  // calculate number of bytes to read
  const s32 bytesToRead = Math::Min(static_cast<s32>(length), m_buffer.length() - m_readOffset);
  
  if (0 < bytesToRead)
  {
    // copy data
    EGE_MEMCPY(data, m_buffer.toAscii() + m_readOffset, static_cast<size_t>(bytesToRead));
  
    // update read offset
    m_readOffset += bytesToRead;
  }

  return bytesToRead;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 StringBuffer::write(const void* data, s64 length)
{
  const String& value = String(reinterpret_cast<const char*>(data), static_cast<s32>(length));

  m_buffer += value;

  return length;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void StringBuffer::clear()
{
  m_buffer.clear();
  m_readOffset = 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END