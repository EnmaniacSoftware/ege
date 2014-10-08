#ifndef EGE_CORE_STRING_H
#define EGE_CORE_STRING_H

#include "EGETypes.h"
#include <string>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class String;
template <typename T> class DynamicArray;
typedef DynamicArray<String> StringArray;
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
   	String& operator+=(char c);
   	String& operator+=(const String& string);

  public:

    /*! Create new object from formatted text. */
    static String Format(const char* text, ...);
    /*! Creates new object from given number. */
    static String FromNumber(s32 value);

  public:

    /*! Creates formatted text. */
    void format(const char* text, ...);

    /*! Converts self to lower-case. */
    String& toLower();
    /*! Converts self to upper-case. */
    String& toUpper();
    /*! Returns ASCII string. */
    const char* toAscii() const;
    /*! Returns TRUE if current string ends with given one. */
    bool endsWith(const String& string) const;
    /*! Returns TRUE if current string starts with given one. */
    bool startsWith(const String& string) const;

    /*! Returns copy of the current string with lowest arg marker replaced with a given string. */
    String arg(const String& string) const;
    /*! Returns copy of the current string with lowest arg marker replaced with a given integer value. */
    String arg(s32 value) const;
    /*! Returns copy of the current string with lowest arg marker replaced with a given unsigned integer value. */
    String arg(u32 value) const;
    /*! Returns copy of the current string with lowest arg marker replaced with a given float value. */
    String arg(float32 value) const;

    /*! Converts to integer. If error is valid, it holds TRUE if error occured during the conversion. */
    s32 toInt(bool* error = NULL) const;
    /*! Converts to 64-bit integer. If error is valid, it holds TRUE if error occured during the conversion. */
    s64 toInt64(bool* error = NULL) const;
    /*! Converts to boolean. If error is valid, it holds TRUE if error occured during the conversion. */
    bool toBool(bool* error = NULL) const;
    /*! Converts to float. If error is valid, it holds TRUE if error occured during the conversion. */
    float32 toFloat(bool* error = NULL) const;

    /*! Splits the string into array of substrings whenever seperator occurs. */
    StringArray split(const String& separator) const;
    /*! Returns a string that has whitespace removed from the start and the end. */
    String trimmed() const;
    /*! Replaces all occurences of the string with another string. 
     *  @param  before String to be replaced.
     *  @param  after  String to be placed.
     *  @return Reference to self.
     */
    String& replaceAll(const String& before, const String& after);
    String& replaceAll(const char* before, const char* after);

  private:

    /*! Arg escape data structure. */
    struct ArgEscapeData
    {
      s32 min_escape;            /*!< Lowest escape sequence number. */
      s32 occurrences;           /*!< Number of occurrences of the lowest escape sequence number. */
    };

  private:

    /*! Finds data about arg escape of the lowest sequence number. */
    ArgEscapeData findArgEscapes() const;
    /*! Replaces args with given string. */
    void replaceArgEscapes(String& out, const String& arg, ArgEscapeData& argData) const;
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