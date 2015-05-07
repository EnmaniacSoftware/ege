#include "Core/String/Interface/StringUtils.h"
#include "EGEStringArray.h"
#include "EGEVector3.h"
#include "EGEDebug.h"
#include <cerrno>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool StringUtils::IsWhiteSpace(char c)
{
  return (' ' == c) || ('\t' == c) || ('\n' == c) || ('\r' == c) || ('\v' == c) || ('\f' == c);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 StringUtils::FromHex(const char* hexInText, bool* error)
{
  // convert
  s32 value = static_cast<s32>(strtol(hexInText, NULL, 16));

  // check for error if necessary
  if (error)
  {
    *error = (errno == ERANGE);
  }

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Color StringUtils::ToColor(const String& string, bool* error)
{
  if (string.isEmpty())
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
  if (3 > sscanf(string.toAscii(), "%f %f %f %f", &color.red, &color.green, &color.blue, &color.alpha) && error)
  {
    *error = true;
  }

  return color;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Rectf StringUtils::ToRectf(const String& string, bool* error)
{
  if (string.isEmpty())
  {
    if (error)
    {
      *error = true;
    }

    return Rectf::INVALID;
  }

  Rectf rect;
  if (4 > sscanf(string.toAscii(), "%f %f %f %f", &rect.x, &rect.y, &rect.width, &rect.height) && error)
  {
    *error = true;
  }

  return rect;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Recti StringUtils::ToRecti(const String& string, bool* error)
{
  if (string.isEmpty())
  {
    if (error)
    {
      *error = true;
    }

    return Recti::INVALID;
  }

  Recti rect;
  if (4 > sscanf(string.toAscii(), "%d %d %d %d", &rect.x, &rect.y, &rect.width, &rect.height) && error)
  {
    *error = true;
  }

  return rect;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector2f StringUtils::ToVector2f(const String& string, bool* error)
{
  if (string.isEmpty())
  {
    if (error)
    {
      *error = true;
    }

    return Vector2f::ZERO;
  }

  Vector2f vec;
  if (2 > sscanf(string.toAscii(), "%f %f", &vec.x, &vec.y) && error)
  {
    *error = true;
  }

  return vec;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector2i StringUtils::ToVector2i(const String& string, bool* error)
{
  if (string.isEmpty())
  {
    if (error)
    {
      *error = true;
    }

    return Vector2i::ZERO;
  }

  Vector2i vec(0, 0);
  if (2 > sscanf(string.toAscii(), "%d %d", &vec.x, &vec.y) && error)
  {
    *error = true;
  }

  return vec;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector3f StringUtils::ToVector3f(const String& string, bool* error)
{
  if (string.isEmpty())
  {
    if (error)
    {
      *error = true;
    }

    return Vector3f::ZERO;
  }

  Vector3f vec(0, 0, 0);
  if (3 > sscanf(string.toAscii(), "%f %f %f", &vec.x, &vec.y, &vec.z) && error)
  {
    *error = true;
  }

  return vec;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector4f StringUtils::ToVector4f(const String& string, bool* error)
{
  if (string.isEmpty())
  {
    if (error)
    {
      *error = true;
    }

    return Vector4f::ZERO;
  }

  Vector4f vec(0, 0, 0, 0);
  if (4 > sscanf(string.toAscii(), "%f %f %f %f", &vec.x, &vec.y, &vec.z, &vec.w) && error)
  {
    *error = true;
  }

  return vec;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector4i StringUtils::ToVector4i(const String& string, bool* error)
{
  if (string.isEmpty())
  {
    if (error)
    {
      *error = true;
    }

    return Vector4i::ZERO;
  }

  Vector4i vec(0, 0, 0, 0);
  if (4 > sscanf(string.toAscii(), "%d %d %d %d", &vec.x, &vec.y, &vec.z, &vec.w) && error)
  {
    *error = true;
  }

  return vec;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Alignment StringUtils::ToAlignment(const String& string, bool* error)
{
  if (string.isEmpty())
  {
    if (error)
    {
      *error = true;
    }

    return ALIGN_TOP_LEFT;
  }

  // split into parts
  const StringArray parts = string.split("-");

  Alignment alignment;
  for (StringArray::ConstIterator it = parts.begin(); it != parts.end(); ++it)
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
      EGE_ASSERT_X(false, "Unknown alignment string");
    }
  }

  return alignment;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Angle StringUtils::ToAngle(const String& string, bool* error)
{
  Angle angle;

  if (string.isEmpty())
  {
    if (error)
    {
      *error = true;
    }

    return angle;
  }

  float32 value;
  if (1 > sscanf(string.toAscii(), "%f", &value) && error)
  {
    *error = true;
  }

  angle.fromDegrees(value);

  return angle;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Time StringUtils::ToTime(const String& string, bool* error)
{
  Time time;

  if (string.isEmpty())
  {
    if (error)
    {
      *error = true;
    }

    return time;
  }

  float32 value;
  if (1 > sscanf(string.toAscii(), "%f", &value) && error)
  {
    *error = true;
  }

  time = value;

  return time;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END