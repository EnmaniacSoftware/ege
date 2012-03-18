#include "Core/String/Stl/String.h"
#include "Core/Containers/Stl/DynamicArray.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <EGEDebug.h>

EGE_NAMESPACE

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
char String::m_buffer[2048] = { '\0' };
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

  s32 value = StringUtils::ToInt(c_str(), error);

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts to 64-bit integer. If error is valid, it holds TRUE if error occured during the conversion. */
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
	va_list arg;
	va_start(arg, text);
	vsprintf(m_buffer, text, arg);
	va_end(arg);

  *this = m_buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Create new object from formatted text. */
String String::Format(const char* text, ...)
{
  String out;

	va_list arg;
	va_start(arg, text);
	vsprintf(m_buffer, text, arg);
	va_end(arg);

  out = m_buffer;

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
/*! Converts to rectangle of integers. If error is valid, it holds TRUE if error occured during the conversion. */
Recti String::toRecti(bool* error) const
{
  if (empty())
  {
    if (error)
    {
      *error = true;
    }

    return Recti::INVALID;
  }

  Recti rect;
  if (4 > sscanf(toAscii(), "%d %d %d %d", &rect.x, &rect.y, &rect.width, &rect.height) && error)
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
/*! Converts to 2D vector of ints. If error is valid, it holds TRUE if error occured during the conversion. */
Vector2i String::toVector2i(bool* error) const
{
  if (empty())
  {
    if (error)
    {
      *error = true;
    }

    return Vector2i::ZERO;
  }

  Vector2i vec(0, 0);
  if (2 > sscanf(toAscii(), "%d %d", &vec.x, &vec.y) && error)
  {
    *error = true;
  }

  return vec;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts to 3D vector of floats. If error is valid, it holds TRUE if error occured during the conversion. */
Vector3f String::toVector3f(bool* error) const
{
  if (empty())
  {
    if (error)
    {
      *error = true;
    }

    return Vector3f::ZERO;
  }

  Vector3f vec(0, 0, 0);
  if (3 > sscanf(toAscii(), "%f %f %f", &vec.x, &vec.y, &vec.z) && error)
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

  Vector4f vec(0, 0, 0, 0);
  if (4 > sscanf(toAscii(), "%f %f %f %f", &vec.x, &vec.y, &vec.z, &vec.w) && error)
  {
    *error = true;
  }

  return vec;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts to 4D vector of signed integers. If error is valid, it holds TRUE if error occured during the conversion. */
Vector4i String::toVector4i(bool* error) const
{
  if (empty())
  {
    if (error)
    {
      *error = true;
    }

    return Vector4i::ZERO;
  }

  Vector4i vec(0, 0, 0, 0);
  if (4 > sscanf(toAscii(), "%d %d %d %d", &vec.x, &vec.y, &vec.z, &vec.w) && error)
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
/*! Finds data about arg escape of the lowest sequence number. */
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
/*! Replaces args with given string. */
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
/*! Returns copy of the current string with lowest arg marker replaced with a given text. */
String String::arg(const String& string) const
{
  ArgEscapeData argEscapes = findArgEscapes();

  String out;
  replaceArgEscapes(out, string, argEscapes);

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns copy of the current string with lowest arg marker replaced with a given integer value. */
String String::arg(s32 value) const
{
  ArgEscapeData argEscapes = findArgEscapes();

  String out;
  replaceArgEscapes(out, String::Format("%d", value), argEscapes);

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Returns copy of the current string with lowest arg marker replaced with a given float value. */
String String::arg(float32 value) const
{
  ArgEscapeData argEscapes = findArgEscapes();

  String out;
  replaceArgEscapes(out, String::Format("%f", value), argEscapes);

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Splits the string into array of substrings whenever seperator occurs. */
StringArray String::split(const String& separator) const
{
  StringArray list;

  size_t pos = 0;
  while (std::string::npos != pos)
  {
    size_t nextPos = this->find(separator, pos);
    if (std::string::npos != nextPos)
    {
      // add to list
      list.push_back(this->substr(pos, nextPos - pos));

      // go to next character after separator
      pos = nextPos + separator.length();
    }
    else
    {
      // check if any string is present after last seperator
      if (pos < length())
      {
        // add last substring to list as well
        list.push_back(this->substr(pos, length() - pos));
      }

      // done
      break;
    }
  }

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts to alignments flag. If error is valid, it holds TRUE if error occured during the conversion. */
Alignment String::toAlignment(bool* error) const
{
  if (empty())
  {
    if (error)
    {
      *error = true;
    }

    return ALIGN_TOP_LEFT;
  }

  // split into parts
  StringArray parts = split("-");

  Alignment alignment;
  for (StringArray::const_iterator it = parts.begin(); it != parts.end(); ++it)
  {
    // remove whitespaces on both sides
    String final = it->trimmed();
    if ("top" == final)
    {
      alignment |= ALIGN_TOP;
    }
    else if ("bottom" == final)
    {
      alignment |= ALIGN_BOTTOM;
    }
    else if ("vcenter" == final)
    {
      alignment |= ALIGN_VCENTER;
    }
    else if ("left" == final)
    {
      alignment |= ALIGN_LEFT;
    }
    else if ("right" == final)
    {
      alignment |= ALIGN_RIGHT;
    }
    else if ("hcenter" == final)
    {
      alignment |= ALIGN_HCENTER;
    }
    else if ("center" == final)
    {
      alignment = ALIGN_CENTER;
    }
    else
    {
      EGE_ASSERT("Unknown alignment string");
    }
  }

  return alignment;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns a string that has whitespace removed from the start and the end. */
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
/*! Converts to angle. If error is valid, it holds TRUE if error occured during the conversion. */
Angle String::toAngle(bool* error) const
{
  Angle angle;

  if (empty())
  {
    if (error)
    {
      *error = true;
    }

    return angle;
  }

  float32 value;
  if (1 > sscanf(toAscii(), "%f", &value) && error)
  {
    *error = true;
  }

  angle.fromDegrees(value);

  return angle;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts to time. If error is valid, it holds TRUE if error occured during the conversion. */
Time String::toTime(bool* error) const
{
  Time time;

  if (empty())
  {
    if (error)
    {
      *error = true;
    }

    return time;
  }

  float32 value;
  if (1 > sscanf(toAscii(), "%f", &value) && error)
  {
    *error = true;
  }

  time = value;

  return time;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
