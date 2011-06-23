#ifndef EGE_CORE_TEXT_H
#define EGE_CORE_TEXT_H

#include <EGE.h>
#include <string>
#include "Core/String/Stl/String.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class EGEText : public std::wstring
{
  public:
    
    EGEText();
    EGEText(const EGEText& text);
    EGEText(const char* string);
    EGEText(const EGEString& string);
   ~EGEText();

   	EGEText& operator=(const EGEText& string);
   	EGEText& operator+=(const EGEText& string);

    /* Create new object from formatted text. */
    static EGEText Format(const char* text, ...);

    /* Creates formatted text. */
    void format(const char* text, ...);
    /* Converts self to lower-case. */
    EGEText& toLower();
    /* Converts self to upper-case. */
    EGEText& toUpper();
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline EGEText operator+(EGEText& left, const EGEText& right)
{
  EGEText string;
  string += left;
  string += right;

  return string;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_TEXT_H