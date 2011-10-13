#include "Core/String/Stl/Text.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <EGEDynamicArray.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Text::Text() : std::wstring()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Text::Text(const Text& text) : std::wstring(text)
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
