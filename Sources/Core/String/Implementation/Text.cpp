#include "Core/String/Interface/Text.h"
#include "EGEDynamicArray.h"
#include "EGEMath.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <stdarg.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const s32 KWorkBufferLength = 512;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Text::Text()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Text::Text(const Text& text) 
: m_value(text.m_value)
{
}
Text::Text(const std::wstring& other)
: m_value(other)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Text::Text(const Char* string) 
: m_value(string)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Text::Text(const Char* string, s32 length)
: m_value(string, length)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Text::Text(const char* string)
{
  std::string temp = string;
  m_value.resize(temp.length());
  std::copy(temp.begin(), temp.end(), m_value.begin());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Text::~Text()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Text& Text::operator=(const Text& other)
{
  m_value = other.m_value;
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Text& Text::operator+=(const Text& other)
{
  m_value += other.m_value;
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Text::operator==(const Text& other) const
{
  return m_value == other.m_value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Text::operator!=(const Text& other) const
{
  return m_value != other.m_value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Text::clear()
{
  m_value.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Text::isEmpty() const
{
  return m_value.empty();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 Text::length() const
{
  return static_cast<s32>(m_value.length());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const Char* Text::data() const
{
  return static_cast<const Char*>(m_value.c_str());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Text& Text::toLower()
{
  // NOTE: explicit casting is needed for XCode4 as there is also template version of 'tolower' in <locale>
  std::transform(m_value.begin(), m_value.end(), m_value.begin(), (int(*)(int)) std::tolower);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Text& Text::toUpper()
{
  // NOTE: explicit casting is needed for XCode4 as there is also template version of 'toupper' in <locale>  
  std::transform(m_value.begin(), m_value.end(), m_value.begin(), (int(*)(int)) std::toupper);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Char Text::at(s32 index) const
{
  EGE_ASSERT((0 <= index) && (index < static_cast<signed>(m_value.size())));
  return m_value[index];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Text::format(const char* text, ...)
{
  char buffer[KWorkBufferLength];

	va_list arg;
	va_start(arg, text);
	vsprintf(buffer, text, arg);
	va_end(arg);

  *this = buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Text Text::Format(const char* text, ...)
{
  char buffer[KWorkBufferLength];

  Text out;

	va_list arg;
	va_start(arg, text);
	vsprintf(buffer, text, arg);
	va_end(arg);

  out = buffer;

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//bool Text::fromString(const String& string)
//{
//  DynamicArray<u32> unicode;
//
//  // go thru all characters
//  s32 i = 0;
//  while (i < string.length())
//  {
//    u32 uni;
//    size_t todo;
//
//    // determine type of character
//    u8 ch = static_cast<u8>(string.at(i++));
//    if (ch <= 0x7F)
//    {
//      uni = ch;
//      todo = 0;
//    }
//    else if (ch <= 0xBF)
//    {
//      // error! not UTF-8 string
//      return false;
//    }
//    else if (ch <= 0xDF)
//    {
//      uni = ch & 0x1F;
//      todo = 1;
//    }
//    else if (ch <= 0xEF)
//    {
//      uni = ch & 0x0F;
//      todo = 2;
//    }
//    else if (ch <= 0xF7)
//    {
//      uni = ch & 0x07;
//      todo = 3;
//    }
//    else
//    {
//      // error! Not UTF-8 String
//      return false;
//    }
//
//    // process all detrmined number of character which contribute to single entity
//    for (size_t j = 0; j < todo; ++j)
//    {
//      if (i == string.length())
//      {
//        // error! Not UTF-8 string
//        return false;
//      }
//
//      ch = string.at(i++);
//      if ((ch < 0x80) || (ch > 0xBF))
//      {
//        // error! Not UTF-8 string
//        return false;
//      }
//
//      uni <<= 6;
//      uni += ch & 0x3F;
//    }
//
//    // check if we came up with valid UTF-16 character
//    if ((0xD800 <= uni) && (0xDFFF >= uni))
//    {
//      // error! Not UTF-8 string
//      return false;
//    }
//
//    if (0x10FFFF < uni)
//    {
//      // error! Not UTF-8 string
//      return false;
//    }
//
//    // ok valid character
//    unicode.push_back(uni);
//  }
//
//  // build final UTF-16 string
//  clear();
//  for (i = 0; i < static_cast<signed>(unicode.size()); ++i)
//  {
//    u32 uni = unicode[i];
//
//    if (uni <= 0xFFFF)
//    {
//      m_value.push_back((wchar_t) uni);
//    }
//    else
//    {
//      uni -= 0x10000;
//
//      m_value.push_back((wchar_t)((uni >> 10) + 0xD800));
//      m_value.push_back((wchar_t)((uni & 0x3FF) + 0xDC00));
//    }
//  }
//
//  return true;
//}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Text::ArgEscapeData Text::findArgEscapes() const
{
  ArgEscapeData data;

  data.min_escape  = Math::MAX_S32;
  data.occurrences = 0;

  // go thru entire string
  std::wstring::const_iterator it = m_value.begin();
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
void Text::replaceArgEscapes(Text& out, const Text& arg, ArgEscapeData& argData) const
{
  // preallocate enough space
  out.m_value.reserve(length() + argData.occurrences * arg.length());

  // go thru entire input string
  std::wstring::const_iterator it = m_value.begin();
  while (it != m_value.end())
  {
    // check if proper args found
    if ('%' == *it)
    {
      std::wstring::const_iterator argBegin = it;

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
//Text Text::arg(const String& string) const
//{
//  ArgEscapeData argEscapes = findArgEscapes();
//
//  Text convertedString(string);
//
//  Text out;
//  replaceArgEscapes(out, convertedString, argEscapes);
//
//  return out;
//}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Text Text::arg(const Text& string) const
{
  ArgEscapeData argEscapes = findArgEscapes();

  Text out;
  replaceArgEscapes(out, string, argEscapes);

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Text Text::arg(s32 value) const
{
  ArgEscapeData argEscapes = findArgEscapes();

  Text out;
  replaceArgEscapes(out, Text::Format("%d", value), argEscapes);

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Text Text::arg(float32 value) const
{
  ArgEscapeData argEscapes = findArgEscapes();

  Text out;
  replaceArgEscapes(out, Text::Format("%f", value), argEscapes);

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 Text::indexOf(const Text& string, s32 position) const
{
  size_t pos = m_value.find(string.m_value, position);
  return (std::wstring::npos != pos) ? static_cast<s32>(pos) : -1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Text& Text::replace(s32 position, s32 length, const Text& string)
{
  EGE_ASSERT((0 <= position) && (position < static_cast<signed>(m_value.size())));
  EGE_ASSERT(0 <= length);

  m_value.replace(position, length, string.m_value);

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Text& Text::replaceAll(const Text& before, const Text& after)
{
  // find first occurence
  size_t pos = m_value.find(before.m_value);
  while (std::wstring::npos != pos)
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

EGE_NAMESPACE_END