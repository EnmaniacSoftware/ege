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
#define WORK_BUFFER_LENGTH (512)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function detrmining if given character is a white space. */
bool IsWhiteSpace(char c)
{
  if ((' ' == c) || ('\t' == c) || ('\n' == c) || ('\r' == c) || ('\v' == c) || ('\f' == c))
  {
    // yes
    return true;
  }

  // no
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String::String() : std::string()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String::String(const String& string) : std::string(string)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String::String(const std::string& string) : std::string(string)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String::String(const char* str) : std::string()
{
  if (NULL != str)
  {
    *this = str;
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
  std::transform(begin(), end(), begin(), (int(*)(int)) std::tolower);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String& String::toUpper()
{
  // NOTE: explicit casting is needed for XCode4 as there is also template version of 'toupper' in <locale>  
  std::transform(begin(), end(), begin(), (int(*)(int)) std::toupper);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String& String::operator=(const char* string)
{
  (*(std::string*) this) = string;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String& String::operator=(const String& string)
{
  (*(std::string*) this) = (*(std::string*) &string);

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String& String::operator+=(const char* string)
{
  (*(std::string*) this) += string;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String& String::operator+=(char c)
{
  (*(std::string*) this) += c;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String& String::operator+=(const String& string)
{
  (*(std::string*) this) += (*(std::string*) &string);

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* String::toAscii() const
{
  return c_str();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 String::toInt(bool* error) const
{
  if (empty())
  {
    if (error)
    {
      *error = true;
    }

    return 0;
  }

  s32 value = StringUtils::ToInt(c_str(), error);

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 String::toInt64(bool* error) const
{
  if (empty())
  {
    if (error)
    {
      *error = true;
    }

    return 0;
  }

  s64 value = StringUtils::ToInt64(c_str(), error);

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool String::toBool(bool* error) const
{
  if (empty())
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

  bool value = (copy == "true");

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
float32 String::toFloat(bool* error) const
{
  if (empty())
  {
    if (error)
    {
      *error = true;
    }

    return 0;
  }

  const char* text = c_str();
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

  return stream.str();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void String::format(const char* text, ...)
{
  char buffer[WORK_BUFFER_LENGTH];

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

  char buffer[WORK_BUFFER_LENGTH];

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
  if (size() >= string.size())
  {
    // check if last characters are the same
    return 0 == compare(size() - string.size(), string.size(), string);
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool String::startsWith(const String& string) const
{
  // check if current string can contain the one to test
  if (size() >= string.size())
  {
    // check if first characters are the same
    return 0 == compare(0, string.size(), string);
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
  String::const_iterator it = begin();
  while (it != end())
  {
    // check if begining of escape found
    if ('%' != *(it++))
    {
      // go on
      continue;
    }

    // check if valid escape identifier
    // NOTE: only [1-99] range is supported
    if ((it != end()) && ('0' <= *it) && ('9' >= *it))
    {
      s32 escape = *it - '0';
      ++it;

      if ((it != end()) && ('0' <= *it) && ('9' >= *it))
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
  out.reserve(length() + argData.occurrences * arg.length());

  // go thru entire input string
  String::const_iterator it = begin();
  while (it != end())
  {
    // check if proper args found
    if ('%' == *it)
    {
      String::const_iterator argBegin = it;

      ++it;

      // check if valid arg escape
      if ((it != end()) && ('0' <= *it) && ('9' >= *it))
      {
        s32 escape = *it - '0';
        ++it;

        if ((it != end()) && ('0' <= *it) && ('9' >= *it))
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
    out.push_back(*it);
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
  String subString;

  size_t pos = 0;
  while (std::string::npos != pos)
  {
    size_t nextPos = this->find(separator, pos);
    if (std::string::npos != nextPos)
    {
      // retrieve substring
      subString = this->substr(pos, nextPos - pos);

      // go to next character after separator
      pos = nextPos + separator.length();
    }
    else
    {
      // check if any string is present after last seperator
      if (pos < length())
      {
        // retrieve substring
        subString = this->substr(pos, length() - pos);
      }

      // no more processing
      pos = nextPos;
    }

    // add to list
    list.push_back(subString);
  }

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String String::trimmed() const
{
  // check if empty string
  if (empty())
  {
    // done
    return *this;
  }

  // check if at the start and end there are non-white characters
  if (!IsWhiteSpace(*begin()) && !IsWhiteSpace(*(end() - 1)))
  {
    // done
    return *this;
  }

  String::const_iterator startPos  = begin();
  String::const_iterator endPos    = end() - 1;

  // skip white spaces from the begining
  while ((startPos <= endPos) && IsWhiteSpace(*startPos))
  {
    ++startPos;
  }

  if (startPos <= endPos)
  {
    // skip white spaces from the end
    while ((endPos != begin()) && IsWhiteSpace(*endPos))
    {
      --endPos;
    }
  }

  return String(std::string(startPos, endPos + 1));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String& String::replaceAll(const String& before, const String& after)
{
  // find first occurence
  size_t pos = this->find(before);
  while (std::string::npos != pos)
  {
    // erase
    this->erase(pos, before.length());

    // insert
    this->insert(pos, after);

    // locate next occurence
    pos = this->find(before, pos + 1);
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

EGE_NAMESPACE_END