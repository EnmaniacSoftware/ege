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
    Text(const char* string);
    Text(const String& string);
   ~Text();

   	Text& operator=(const Text& string);
   	Text& operator+=(const Text& string);

    /* Create new object from formatted text. */
    static Text Format(const char* text, ...);

    /* Creates formatted text. */
    void format(const char* text, ...);
    /* Converts self to lower-case. */
    Text& toLower();
    /* Converts self to upper-case. */
    Text& toUpper();

  private:

    /* Converts from given UTF-8 string into UTF-16. */
    bool fromString(const String& string);
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