#ifndef EGE_CORE_STRING_H
#define EGE_CORE_STRING_H

#include <EGETypes.h>
#include <EGERect.h>
#include <string>
#include "Core/Graphics/Color.h"
#include "Core/Math/Vector2.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class String : public std::string
{
  public:
    
    String();
    String(const String& string);
    String(const std::string& string);
    String(const char* string);
   ~String();

   	String& operator=(const char* string);
   	String& operator=(const String& string);
   	String& operator+=(const char* string);
   	String& operator+=(const String& string);

  public:

    /* Create new object from formatted text. */
    static String Format(const char* text, ...);
    /* Creates new object from given number. */
    static String FromNumber(s32 value);

  public:

    /* Creates formatted text. */
    void format(const char* text, ...);

    /* Converts self to lower-case. */
    String& toLower();
    /* Converts self to upper-case. */
    String& toUpper();
    /* Returns ASCII string. */
    const char* toAscii() const;
    /* Returns TRUE if current string ends with given one. */
    bool endsWith(const String& string) const;
    /* Returns TRUE if current string starts with given one. */
    bool startsWith(const String& string) const;

    /* Converts to integer. If error is valid, it holds TRUE if error occured during the conversion. */
    s32 toInt(bool* error = NULL) const;
    /* Converts to boolean. If error is valid, it holds TRUE if error occured during the conversion. */
    bool toBool(bool* error = NULL) const;
    /* Converts to float. If error is valid, it holds TRUE if error occured during the conversion. */
    float32 toFloat(bool* error = NULL) const;
    /* Converts to color. If error is valid, it holds TRUE if error occured during the conversion. */
    Color toColor(bool* error = NULL) const;
    /* Converts to rectangle of floats. If error is valid, it holds TRUE if error occured during the conversion. */
    Rectf toRectf(bool* error = NULL) const;
    /* Converts to 2D vector of floats. If error is valid, it holds TRUE if error occured during the conversion. */
    Vector2f toVector2f(bool* error = NULL) const;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline String operator + (const String& left, const char* right)
{
  String string;
  string += left;
  string += right;

  return string;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline String operator + (String& left, const String& right)
{
  String string;
  string += left;
  string += right;

  return string;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_STRING_H