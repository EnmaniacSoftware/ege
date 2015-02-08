#include "Core/String/Interface/STL/String.h"
#include "EGEStringArray.h"
#include "EGEStringUtils.h"
#include "EGEDebug.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <stdarg.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const s32 KWorkBufferLength = 512;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function detrmining if given character is a white space. */
static bool IsWhiteSpace(char c)
{
  return (' ' == c) || ('\t' == c) || ('\n' == c) || ('\r' == c) || ('\v' == c) || ('\f' == c);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String::String()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String::String(const String& other) : m_value(other.m_value)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String::String(const std::string& string) : m_value(string)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String::String(const char* str)
{
  if (NULL != str)
  {
    m_value = str;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String::~String()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String& String::toLower()
{
  // NOTE: explicit casting is needed for XCode4 as there is also template version of 'tolower' in <locale>
  std::transform(m_value.begin(), m_value.end(), m_value.begin(), (int(*)(int)) std::tolower);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String& String::toUpper()
{
  // NOTE: explicit casting is needed for XCode4 as there is also template version of 'toupper' in <locale>  
  std::transform(m_value.begin(), m_value.end(), m_value.begin(), (int(*)(int)) std::toupper);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String& String::operator=(const char* string)
{
  m_value = string;
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String& String::operator=(const String& string)
{
  m_value = string.m_value;
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String& String::operator+=(const char* string)
{
  m_value += string;
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String& String::operator+=(char c)
{
  m_value += c;
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String& String::operator+=(const String& string)
{
  m_value += string.m_value;
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool String::operator==(const String& other) const
{
  return m_value == other.m_value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool String::operator==(const char* other) const
{
  return m_value == other;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool String::operator!=(const String& other) const
{
  return m_value != other.m_value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool String::operator!=(const char* other) const
{
  return m_value != other;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool String::operator<(const String& other) const
{
  return m_value < other.m_value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* String::toAscii() const
{
  return m_value.c_str();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void String::clear()
{
  m_value.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool String::isEmpty() const
{
  return m_value.empty();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 String::indexOf(const String& string, s32 position) const
{
  size_t pos = m_value.find(string.m_value, position);
  return (std::string::npos != pos) ? static_cast<s32>(pos) : -1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 String::lastIndexOf(const String& string) const
{
  return static_cast<s32>(m_value.rfind(string.m_value));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String String::subString(s32 position, s32 length) const
{
  return m_value.substr(position, (0 > length) ? std::string::npos : length);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
char String::at(s32 index) const
{
  EGE_ASSERT((0 <= index) && (index < static_cast<signed>(m_value.size())));
  return m_value[index];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 String::toInt(bool* error) const
{
  if (m_value.empty())
  {
    if (error)
    {
      *error = true;
    }

    return 0;
  }

  s32 value = StringUtils::ToInt(m_value.c_str(), error);

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 String::toInt64(bool* error) const
{
  if (m_value.empty())
  {
    if (error)
    {
      *error = true;
    }

    return 0;
  }

  s64 value = StringUtils::ToInt64(m_value.c_str(), error);

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool String::toBool(bool* error) const
{
  if (m_value.empty())
  {
    if (error)
    {
      *error = true;
    }

    return false;
  }

  // make lowercase copy
  String copy = *this;
  copy.toLower();

  // assume FALSE
  bool value = false;

  // check if TRUE
  if ("true" == copy)
  {
    value = true;
  }
  // check if NOT FALSE
  else if ("false" != copy)
  {
    // error, not FALSE and not TRUE
    if (error)
    {
      *error = true;
    }
  }

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 String::toFloat(bool* error) const
{
  if (m_value.empty())
  {
    if (error)
    {
      *error = true;
    }

    return 0;
  }

  const char* text = m_value.c_str();
  char* end;

  float32 value = (float32) strtod(text, &end);

  // check for error
  if (error && (0 != *end))
  {
    *error = true;
  }

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String String::FromNumber(s32 value)
{
  std::stringstream stream;

  stream << value;

  return String(stream.str());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void String::format(const char* text, ...)
{
  char buffer[KWorkBufferLength];

	va_list arg;
	va_start(arg, text);
	vsprintf(buffer, text, arg);
	va_end(arg);

  *this = buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String String::Format(const char* text, ...)
{
  String out;

  char buffer[KWorkBufferLength];

	va_list arg;
	va_start(arg, text);
	vsprintf(buffer, text, arg);
	va_end(arg);

  out = buffer;

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool String::endsWith(const String& string) const
{
  // check if current string can contain the one to test
  if (m_value.size() >= string.m_value.size())
  {
    // check if last characters are the same
    return 0 == m_value.compare(m_value.size() - string.m_value.size(), string.m_value.size(), string.m_value);
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool String::startsWith(const String& string) const
{
  // check if current string can contain the one to test
  if (m_value.size() >= string.m_value.size())
  {
    // check if first characters are the same
    return 0 == m_value.compare(0, string.m_value.size(), string.m_value);
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String::ArgEscapeData String::findArgEscapes() const
{
  ArgEscapeData data;

  data.min_escape  = Math::MAX_S32;
  data.occurrences = 0;

  // go thru entire string
  std::string::const_iterator it = m_value.begin();
  while (it != m_value.end())
  {
    // check if begining of escape found
    if ('%' != *(it++))
    {
      // go on
      continue;
    }

    // check if valid escape identifier
    // NOTE: only [1-99] range is supported
    if ((it != m_value.end()) && ('0' <= *it) && ('9' >= *it))
    {
      s32 escape = *it - '0';
      ++it;

      if ((it != m_value.end()) && ('0' <= *it) && ('9' >= *it))
      {
        escape = escape * 10 + *it - '0';
        ++it;
      }

      // check if new smallest escape found
      if (data.min_escape > escape)
      {
        // store it and reset occurences
        data.min_escape = escape;
        data.occurrences = 1;
      }
      else if (data.min_escape == escape)
      {
        // already min, increase occurences
        ++data.occurrences;
      }
    }
  }

  return data;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void String::replaceArgEscapes(String& out, const String& arg, ArgEscapeData& argData) const
{
  // preallocate enough space
  out.m_value.reserve(m_value.length() + argData.occurrences * arg.m_value.length());

  // go thru entire input string
  std::string::const_iterator it = m_value.begin();
  while (it != m_value.end())
  {
    // check if proper args found
    if ('%' == *it)
    {
      std::string::const_iterator argBegin = it;

      ++it;

      // check if valid arg escape
      if ((it != m_value.end()) && ('0' <= *it) && ('9' >= *it))
      {
        s32 escape = *it - '0';
        ++it;

        if ((it != m_value.end()) && ('0' <= *it) && ('9' >= *it))
        {
          escape = escape * 10 + *it - '0';
          ++it;
        }

        // check if current escape is the one which needs to be replaced
        if (argData.min_escape == escape)
        {
          // replace
          out += arg;
          continue;
        }
        else
        {
          // thats not the one, move back and copy chars instead
          it = argBegin;
        }
      }
      else
      {
        // thats not the one, move back and copy chars instead
        it = argBegin;
      }
    }
    
    // just copy
    out.m_value.push_back(*it);
    ++it;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String String::arg(const String& string) const
{
  ArgEscapeData argEscapes = findArgEscapes();

  String out;
  replaceArgEscapes(out, string, argEscapes);

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String String::arg(s32 value) const
{
  ArgEscapeData argEscapes = findArgEscapes();

  String out;
  replaceArgEscapes(out, String::Format("%d", value), argEscapes);

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String String::arg(u32 value) const
{
  ArgEscapeData argEscapes = findArgEscapes();

  String out;
  replaceArgEscapes(out, String::Format("%u", value), argEscapes);

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String String::arg(float32 value) const
{
  ArgEscapeData argEscapes = findArgEscapes();

  String out;
  replaceArgEscapes(out, String::Format("%f", value), argEscapes);

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
StringArray String::split(const String& separator) const
{
  StringArray list;
  std::string subString;

  size_t pos = 0;
  while (std::string::npos != pos)
  {
    size_t nextPos = m_value.find(separator.m_value, pos);
    if (std::string::npos != nextPos)
    {
      // retrieve substring
      subString = m_value.substr(pos, nextPos - pos);

      // go to next character after separator
      pos = nextPos + separator.m_value.length();
    }
    else
    {
      // check if any string is present after last seperator
      if (pos < m_value.length())
      {
        // retrieve substring
        subString = m_value.substr(pos, m_value.length() - pos);
      }

      // no more processing
      pos = nextPos;
    }

    // add to list
    list.push_back(String(subString));
  }

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String String::trimmed() const
{
  // check if empty string
  if (m_value.empty())
  {
    // done
    return *this;
  }

  // check if at the start and end there are non-white characters
  if (!IsWhiteSpace(*m_value.begin()) && !IsWhiteSpace(*(m_value.end() - 1)))
  {
    // done
    return *this;
  }

  std::string::const_iterator startPos  = m_value.begin();
  std::string::const_iterator endPos    = m_value.end() - 1;

  // skip white spaces from the begining
  while ((startPos <= endPos) && IsWhiteSpace(*startPos))
  {
    ++startPos;
  }

  if (startPos <= endPos)
  {
    // skip white spaces from the end
    while ((endPos != m_value.begin()) && IsWhiteSpace(*endPos))
    {
      --endPos;
    }
  }

  return String(std::string(startPos, endPos + 1));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String& String::replace(s32 position, s32 length, const String& string)
{
  EGE_ASSERT((0 <= position) && (position < static_cast<signed>(m_value.size())));
  EGE_ASSERT(0 <= length);

  m_value.replace(position, length, string.m_value);

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String& String::replaceAll(const String& before, const String& after)
{
  // find first occurence
  size_t pos = m_value.find(before.m_value);
  while (std::string::npos != pos)
  {
    // erase
    m_value.erase(pos, before.m_value.length());

    // insert
    m_value.insert(pos, after.m_value);

    // locate next occurence
    pos = m_value.find(before.m_value, pos + 1);
  }

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String& String::replaceAll(const char* before, const char* after)
{
  const String beforeString(before);
  const String afterString(after);

  return replaceAll(beforeString, afterString);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 String::length() const
{
  return static_cast<s32>(m_value.length());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END
