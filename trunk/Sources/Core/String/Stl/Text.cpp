#include "Core/String/Stl/Text.h"
#include <sstream>
#include <algorithm>
#include <cctype>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEText::EGEText() : std::wstring()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEText::EGEText(const EGEText& text) : std::wstring(text)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEText::EGEText(const char* string) : std::wstring()
{
  EGEString temp = string;
  resize(temp.length());
  std::copy(temp.begin(), temp.end(), begin());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEText::EGEText(const EGEString& string) : std::wstring(string.length(), L' ')
{
  std::copy(string.begin(), string.end(), begin());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEText::~EGEText()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts self to lower-case. */
EGEText& EGEText::toLower()
{
  std::transform(begin(), end(), begin(), std::tolower);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts self to upper-case. */
EGEText& EGEText::toUpper()
{
  std::transform(begin(), end(), begin(), std::toupper);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEText& EGEText::operator=(const EGEText& string)
{
  (*(std::wstring*) this) = (*(std::wstring*) &string);

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEText& EGEText::operator+=(const EGEText& string)
{
  (*(std::wstring*) this) += (*(std::wstring*) &string);

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates formatted text. */
void EGEText::format(const char* text, ...)
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
EGEText EGEText::Format(const char* text, ...)
{
  EGEText out;

  char buffer[256];

	va_list arg;
	va_start(arg, text);
	vsprintf(buffer, text, arg);
	va_end(arg);

  out = buffer;

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
