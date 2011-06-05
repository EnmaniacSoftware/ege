#ifndef EGE_CORE_STRING_H
#define EGE_CORE_STRING_H

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class EGEString : public std::string
{
  public:
    
    EGEString();
    EGEString(const EGEString& string);
    EGEString(const std::string& string);
    EGEString(const char* string);
   ~EGEString();

   	EGEString& operator=(const char* string);
   	EGEString& operator=(const EGEString& string);
   	EGEString& operator+=(const char* string);
   	EGEString& operator+=(const EGEString& string);

    /* Create new object from formatted text. */
    static EGEString Format(const char* text, ...);
    /* Creates new object from given number. */
    static EGEString FromNumber(s32 value);

    /* Creates formatted text. */
    void format(const char* text, ...);

    /* Converts self to lower-case. */
    EGEString& toLower();
    /* Converts self to upper-case. */
    EGEString& toUpper();
    /* Returns ASCII string. */
    const char* toAscii() const;

    /* Converts to integer. If error is valid, it holds TRUE if error occured during the conversion. */
    s32  toInt(bool* error = NULL) const;
    /* Converts to boolean. If error is valid, it holds TRUE if error occured during the conversion. */
    bool toBool(bool* error = NULL) const;
    /* Converts to float. If error is valid, it holds TRUE if error occured during the conversion. */
    float32 toFloat(bool* error = NULL) const;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline EGEString operator+(const EGEString& left, const char* right)
{
  EGEString string;
  string += left;
  string += right;

  return string;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline EGEString operator+(EGEString& left, const EGEString& right)
{
  EGEString string;
  string += left;
  string += right;

  return string;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_STRING_H