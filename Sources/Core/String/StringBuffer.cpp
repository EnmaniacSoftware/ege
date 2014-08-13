#include "Core/String/StringBuffer.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(StringBuffer)
EGE_DEFINE_DELETE_OPERATORS(StringBuffer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
StringBuffer::StringBuffer() : Object(EGE_OBJECT_UID_STRING_BUFFER)
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
ISerializable& StringBuffer::operator << (u8 value)
{
  m_buffer += String::Format("%u", value);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& StringBuffer::operator << (s8 value)
{
  m_buffer += String::Format("%d", value);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& StringBuffer::operator << (u16 value)
{
  m_buffer += String::Format("%u", value);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& StringBuffer::operator << (s16 value)
{
  m_buffer += String::Format("%d", value);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& StringBuffer::operator << (u32 value)
{
  m_buffer += String::Format("%u", value);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& StringBuffer::operator << (s32 value)
{
  m_buffer += String::Format("%d", value);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& StringBuffer::operator << (u64 value)
{
  m_buffer += String::Format("%llu", value);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& StringBuffer::operator << (s64 value)
{
  m_buffer += String::Format("%lld", value);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& StringBuffer::operator << (bool value)
{
  m_buffer += String::Format("%s", value ? "true" : "false");
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& StringBuffer::operator << (float32 value)
{
  m_buffer += String::Format("%f", value);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& StringBuffer::operator << (float64 value)
{
  m_buffer += String::Format("%f", value);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& StringBuffer::operator << (const char* value)
{
  m_buffer += value;
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& StringBuffer::operator << (const String& value)
{
  m_buffer += value;
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& StringBuffer::operator >> (u8& value)
{
  EGE_UNUSED(value);
  EGE_ASSERT_X(false, "Unsupported");
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& StringBuffer::operator >> (s8& value)
{
  EGE_UNUSED(value);
  EGE_ASSERT_X(false, "Unsupported");
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& StringBuffer::operator >> (u16& value)
{
  EGE_UNUSED(value);
  EGE_ASSERT_X(false, "Unsupported");
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& StringBuffer::operator >> (s16& value)
{
  EGE_UNUSED(value);
  EGE_ASSERT_X(false, "Unsupported");
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& StringBuffer::operator >> (u32& value)
{
  EGE_UNUSED(value);
  EGE_ASSERT_X(false, "Unsupported");
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& StringBuffer::operator >> (s32& value)
{
  EGE_UNUSED(value);
  EGE_ASSERT_X(false, "Unsupported");
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& StringBuffer::operator >> (u64& value)
{
  EGE_UNUSED(value);
  EGE_ASSERT_X(false, "Unsupported");
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& StringBuffer::operator >> (s64& value)
{
  EGE_UNUSED(value);
  EGE_ASSERT_X(false, "Unsupported");
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& StringBuffer::operator >> (bool& value)
{
  EGE_UNUSED(value);
  EGE_ASSERT_X(false, "Unsupported");
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& StringBuffer::operator >> (float32& value)
{
  EGE_UNUSED(value);
  EGE_ASSERT_X(false, "Unsupported");
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& StringBuffer::operator >> (float64& value)
{
  EGE_UNUSED(value);
  EGE_ASSERT_X(false, "Unsupported");
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& StringBuffer::operator >> (const char* value)
{
  EGE_UNUSED(value);
  EGE_ASSERT_X(false, "Unsupported");
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ISerializable& StringBuffer::operator >> (String& value)
{
  EGE_UNUSED(value);
  EGE_ASSERT_X(false, "Unsupported");
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END