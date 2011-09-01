#include "Core/String/Stl/String.h"
#include <sstream>
#include <algorithm>
#include <cctype>

EGE_NAMESPACE

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
String::String(const char* string) : std::string(string ? string : "")
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
String::~String()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts self to lower-case. */
String& String::toLower()
{
  std::transform(begin(), end(), begin(), std::tolower);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts self to upper-case. */
String& String::toUpper()
{
  std::transform(begin(), end(), begin(), std::toupper);
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
String& String::operator+=(const String& string)
{
  (*(std::string*) this) += (*(std::string*) &string);

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns ASCII string. */
const char* String::toAscii() const
{
  return c_str();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts to integer. If error is valid, it holds TRUE if error occured during the conversion. */
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
/*! Converts to float. If error is valid, it holds TRUE if error occured during the conversion. */
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
/*! Creates new object from given number. */
String String::FromNumber(s32 value)
{
  std::stringstream stream;

  stream << value;

  return stream.str();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates formatted text. */
void String::format(const char* text, ...)
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
String String::Format(const char* text, ...)
{
  String out;

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
Color String::toColor(bool* error) const
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
Rectf String::toRectf(bool* error) const
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
Vector2f String::toVector2f(bool* error) const
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
/*! Converts to 4D vector of floats. If error is valid, it holds TRUE if error occured during the conversion. */
Vector4f String::toVector4f(bool* error) const
{
  if (empty())
  {
    if (error)
    {
      *error = true;
    }

    return Vector4f::ZERO;
  }

  // NOTE: at least 3 coords are required
  Vector4f vec(0, 0, 0, 1);
  if (3 > sscanf(toAscii(), "%f %f %f %f", &vec.x, &vec.y, &vec.z, &vec.w) && error)
  {
    *error = true;
  }

  return vec;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if current string ends with given one. */
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
/*! Returns TRUE if current string starts with given one. */
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
