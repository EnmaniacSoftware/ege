#include "Core/String/Stl/String.h"
#include <sstream>
#include <algorithm>
#include <cctype>

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
  if (empty())
  {
    if (error)
    {
      *error = true;
    }

    return false;
  }

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
/*! Creates new object from given number. */
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
/*! Create new object from formatted text. */
EGEString EGEString::Format(const char* text, ...)
{
  EGEString out;

  char buffer[256];

	va_list arg;
	va_start(arg, text);
	vsprintf(buffer, text, arg);
	va_end(arg);

  out = buffer;

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts to color. If error is valid, it holds TRUE if error occured during the conversion. */
Color EGEString::toColor(bool* error) const
{
  if (empty())
  {
    if (error)
    {
      *error = true;
    }

    return Color::NONE;
  }

  // make sure alpha is set to opaque in case only RGB is set
  Color color = Color::BLACK;

  // check if not even RGB was able to be read
  if (3 > sscanf(toAscii(), "%f %f %f %f", &color.red, &color.green, &color.blue, &color.alpha) && error)
  {
    *error = true;
  }

  return color;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts to rectangle of floats. If error is valid, it holds TRUE if error occured during the conversion. */
Rectf EGEString::toRectf(bool* error) const
{
  if (empty())
  {
    if (error)
    {
      *error = true;
    }

    return Rectf::INVALID;
  }

  Rectf rect;
  if (4 > sscanf(toAscii(), "%f %f %f %f", &rect.x, &rect.y, &rect.width, &rect.height) && error)
  {
    *error = true;
  }

  return rect;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts to 2D vector of floats. If error is valid, it holds TRUE if error occured during the conversion. */
Vector2f EGEString::toVector2f(bool* error) const
{
  if (empty())
  {
    if (error)
    {
      *error = true;
    }

    return Vector2f::ZERO;
  }

  Vector2f vec;
  if (2 > sscanf(toAscii(), "%f %f", &vec.x, &vec.y) && error)
  {
    *error = true;
  }

  return vec;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
