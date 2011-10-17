#include "Core/String/Stl/Text.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <EGEDynamicArray.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Text Text::EMPTY = "";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Text::Text() : std::wstring()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Text::Text(const Text& text) : std::wstring(text)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Text::Text(const Char* string) : std::wstring(string)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Text::Text(const char* string) : std::wstring()
{
  String temp = string;
  resize(temp.length());
  std::copy(temp.begin(), temp.end(), begin());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Text::Text(const String& string) : std::wstring(string.length(), L' ')
{
  fromString(string);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Text::~Text()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts self to lower-case. */
Text& Text::toLower()
{
  std::transform(begin(), end(), begin(), std::tolower);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts self to upper-case. */
Text& Text::toUpper()
{
  std::transform(begin(), end(), begin(), std::toupper);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Text& Text::operator=(const Text& string)
{
  (*(std::wstring*) this) = (*(std::wstring*) &string);

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Text& Text::operator+=(const Text& string)
{
  (*(std::wstring*) this) += (*(std::wstring*) &string);

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates formatted text. */
void Text::format(const char* text, ...)
{
  char buffer[256];

	va_list arg;
	va_start(arg, text);
	vsprintf(buffer, text, arg);
	va_end(arg);

  *this = buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Create new object from formatted text. */
Text Text::Format(const char* text, ...)
{
  Text out;

  char buffer[256];

	va_list arg;
	va_start(arg, text);
	vsprintf(buffer, text, arg);
	va_end(arg);

  out = buffer;

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts from given UTF-8 string into UTF-16. */
bool Text::fromString(const String& string)
{
  DynamicArray<u32> unicode;

  // go thru all characters
  size_t i = 0;
  while (i < string.size())
  {
    u32 uni;
    size_t todo;

    // determine type of character
    u8 ch = string[i++];
    if (ch <= 0x7F)
    {
      uni = ch;
      todo = 0;
    }
    else if (ch <= 0xBF)
    {
      // error! not UTF-8 string
      return false;
    }
    else if (ch <= 0xDF)
    {
      uni = ch & 0x1F;
      todo = 1;
    }
    else if (ch <= 0xEF)
    {
      uni = ch & 0x0F;
      todo = 2;
    }
    else if (ch <= 0xF7)
    {
        uni = ch & 0x07;
        todo = 3;
    }
    else
    {
      // error! Not UTF-8 String
      return false;
    }

    // process all detrmined number of character which contribute to single entity
    for (size_t j = 0; j < todo; ++j)
    {
      if (i == string.size())
      {
        // error! Not UTF-8 string
        return false;
      }

      u8 ch = string[i++];
      if (ch < 0x80 || ch > 0xBF)
      {
        // error! Not UTF-8 string
        return false;
      }

      uni <<= 6;
      uni += ch & 0x3F;
    }

    // check if we came up with valid UTF-16 character
    if ((0xD800 <= uni) && (0xDFFF >= uni))
    {
      // error! Not UTF-8 string
      return false;
    }

    if (0x10FFFF < uni)
    {
      // error! Not UTF-8 string
      return false;
    }

    // ok valid character
    unicode.push_back(uni);
  }

  // build final UTF-16 string
  clear();
  for (size_t i = 0; i < unicode.size(); ++i)
  {
    u32 uni = unicode[i];

    if (uni <= 0xFFFF)
    {
      push_back((wchar_t) uni);
    }
    else
    {
      uni -= 0x10000;

      push_back((wchar_t)((uni >> 10) + 0xD800));
      push_back((wchar_t)((uni & 0x3FF) + 0xDC00));
    }
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Finds data about arg escape of the lowest sequence number. */
Text::ArgEscapeData Text::findArgEscapes() const
{
  ArgEscapeData data;

  data.min_escape  = Math::MAX_INT;
  data.occurrences = 0;

  // go thru entire string
  Text::const_iterator it = begin();
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
/*! Replaces args with given string. */
void Text::replaceArgEscapes(Text& out, const String& arg, ArgEscapeData& argData) const
{
  // preallocate enough space
  out.reserve(length() + argData.occurrences * arg.length());

  // go thru entire input string
  Text::const_iterator it = begin();
  while (it != end())
  {
    // check if proper args found
    if ('%' == *it)
    {
      Text::const_iterator argBegin = it;

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
/*! Returns copy of the current string with lowest arg marker replaced with a given string. */
Text Text::arg(const String& string) const
{
  ArgEscapeData argEscapes = findArgEscapes();

  Text out;
  replaceArgEscapes(out, string, argEscapes);

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns copy of the current string with lowest arg marker replaced with a given integer value. */
Text Text::arg(s32 value) const
{
  ArgEscapeData argEscapes = findArgEscapes();

  Text out;
  replaceArgEscapes(out, String::Format("%d", value), argEscapes);

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns copy of the current string with lowest arg marker replaced with a given integer value. */
Text Text::arg(int value) const
{
  ArgEscapeData argEscapes = findArgEscapes();

  Text out;
  replaceArgEscapes(out, String::Format("%d", value), argEscapes);

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns copy of the current string with lowest arg marker replaced with a given float value. */
Text Text::arg(float32 value) const
{
  ArgEscapeData argEscapes = findArgEscapes();

  Text out;
  replaceArgEscapes(out, String::Format("%f", value), argEscapes);

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
