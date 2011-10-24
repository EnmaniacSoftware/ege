#ifndef EGE_CORE_TEXT_H
#define EGE_CORE_TEXT_H

#include <EGETypes.h>
#include <string>
#include "Core/String/Stl/String.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Text : public std::wstring
{
  public:
    
    Text();
    Text(const Text& text);
    Text(const Char* string);
    Text(const char* string);
    Text(const String& string);
   ~Text();

   	Text& operator=(const Text& string);
   	Text& operator+=(const Text& string);

  public:

    /* Create new object from formatted text. */
    static Text Format(const char* text, ...);

  public:

    /* Creates formatted text. */
    void format(const char* text, ...);
    /* Converts self to lower-case. */
    Text& toLower();
    /* Converts self to upper-case. */
    Text& toUpper();

    /* Returns copy of the current string with lowest arg marker replaced with a given string. */
    Text arg(const String& string) const;
    /* Returns copy of the current string with lowest arg marker replaced with a given integer value. */
    Text arg(s32 value) const;
    /* Returns copy of the current string with lowest arg marker replaced with a given float value. */
    Text arg(float32 value) const;

  public:

    /*! Empty text. */
    static Text EMPTY;

  private:

    /*! Arg escape data structure. */
    struct ArgEscapeData
    {
      s32 min_escape;            /*!< Lowest escape sequence number. */
      s32 occurrences;           /*!< Number of occurrences of the lowest escape sequence number. */
    };

  private:

    /* Converts from given UTF-8 string into UTF-16. */
    bool fromString(const String& string);
    /* Finds data about arg escape of the lowest sequence number. */
    ArgEscapeData findArgEscapes() const;
    /* Replaces args with given string. */
    void replaceArgEscapes(Text& out, const String& arg, ArgEscapeData& argData) const;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline Text operator+(Text& left, const Text& right)
{
  Text string;
  string += left;
  string += right;

  return string;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_TEXT_H