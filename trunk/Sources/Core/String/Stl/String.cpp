#include "EGE.h"
#include "Core/String/Stl/String.h"
#include <sstream>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEString::EGEString() : std::string()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEString::EGEString(const EGEString& string) : std::string(string)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEString::EGEString(const std::string& string) : std::string(string)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEString::EGEString(const char* string) : std::string(string ? string : "")
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEString::~EGEString()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts self to lower-case. */
EGEString& EGEString::toLower()
{
  std::transform(begin(), end(), begin(), std::tolower);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts self to upper-case. */
EGEString& EGEString::toUpper()
{
  std::transform(begin(), end(), begin(), std::toupper);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEString& EGEString::operator=(const char* string)
{
  (*(std::string*) this) = string;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEString& EGEString::operator=(const EGEString& string)
{
  (*(std::string*) this) = (*(std::string*) &string);

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEString& EGEString::operator+=(const char* string)
{
  (*(std::string*) this) += string;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEString& EGEString::operator+=(const EGEString& string)
{
  (*(std::string*) this) += (*(std::string*) &string);

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns ASCII string. */
const char* EGEString::toAscii() const
{
  return c_str();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts to integer. If error is valid, it holds TRUE if error occured during the conversion. */
s32 EGEString::toInt(bool* error) const
{
  const char* text = c_str();
  char* end;

  s32 value = strtol(text, &end, 10);

  // check for error
  if (error && (0 != *end))
  {
    *error = true;
  }

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts to boolean. If error is valid, it holds TRUE if error occured during the conversion. */
bool EGEString::toBool(bool* error) const
{
  EGE_UNUSED(error);

  // make lowercase copy
  EGEString copy = *this;
  copy.toLower();

  bool value = (copy == "true");

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts to float. If error is valid, it holds TRUE if error occured during the conversion. */
float32 EGEString::toFloat(bool* error) const
{
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
EGEString EGEString::FromNumber(s32 value)
{
  std::stringstream stream;

  stream << value;

  return stream.str();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates formatted text. */
void EGEString::format(const char* text, ...)
{
  char buffer[256];

	va_list arg;
	va_start(arg, text);
	vsprintf(buffer, text, arg);
	va_end(arg);

  *this = buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
