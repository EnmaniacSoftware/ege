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
class String
{
  public:
    
    String();
    String(const String& other);
    String(const std::string& string);
    String(const char* string, s32 length = -1);
   ~String();

  operators:

   	String& operator=(const char* string);
   	String& operator=(const String& string);

   	String& operator+=(const char* string);
   	String& operator+=(char c);
   	String& operator+=(const String& string);

    bool operator==(const String& other) const;
    bool operator==(const char* other) const;
    
    bool operator!=(const String& other) const;
    bool operator!=(const char* other) const;

	  bool operator<(const String& other) const;

  public:

    /*! Create new object from formatted text. */
    // TAGE - this probably can be removed
    static String Format(const char* text, ...);
    /*! Creates new object from given number. */
    // TAGE - this probably can be removed
    static String FromNumber(s32 value);

  public:

    /*! Returns length of the string (in bytes). 
     *  @note Returned value does not contain NULL termination character.
     */
    s32 length() const;
    /*! Clears the string. */
    void clear();
    /*! Returns TRUE if string is empty. */
    bool isEmpty() const;

    /*! Returns the index position of the first occurrence of the string in this string, searching forward from given index position.
     *  @param  string    String to look for.
     *  @param  position  Index position the search is to be started from.
     *  @return 0-based index of the begining of a given string within the current one or negative value if string is not found.
     */
    s32 indexOf(const String& string, s32 position = 0) const;
    /*! Returns an index position of the last occurence of the given string.
     *  @param  string  String to look for its last occurance.
     *  @return 0-based index of the begining of a given string within the current one or negative value if string is not found.
     */
    s32 lastIndexOf(const String& string) const;

    /*! Returns TRUE if current string ends with given one. */
    bool endsWith(const String& string) const;
    /*! Returns TRUE if current string starts with given one. */
    bool startsWith(const String& string) const;

    /*! Returns character at a given index.
     *  @param  index   0-based index of character.
     *  @return Character found.
     */
    char at(s32 index) const;

    /*! Returns a string that contains characters of this string, starting at the specified position index.
     *  @param  position  Starting index within a current string.
     *  @param  length    Length of the new string. If negative, all characters of the current string from given position will be used.
     *  @return Created string.
     */
    String subString(s32 position, s32 length = -1) const;

    /*! Creates formatted text. */
    void format(const char* text, ...);

    /*! Converts self to lower-case. */
    String& toLower();
    /*! Converts self to upper-case. */
    String& toUpper();
    /*! Returns ASCII string. */
    const char* toAscii() const;

    /*! Returns copy of the current string with lowest arg marker replaced with a given string. */
    String arg(const String& string) const;
    /*! Returns copy of the current string with lowest arg marker replaced with a given integer value. */
    String arg(s32 value) const;
    /*! Returns copy of the current string with lowest arg marker replaced with a given unsigned integer value. */
    String arg(u32 value) const;
    /*! Returns copy of the current string with lowest arg marker replaced with a given float value. */
    String arg(float32 value) const;

    /*! Converts current string to 32-bit signed integer.
     *  @param  error Optional placeholder for error flag.
     *  @return Converted value. In case of an error, this value should not be used.
     *  @note If optional error placeholder is given and no error occurs during the conversion, its value is not modifed.
     */
    s32 toInt(bool* error = NULL) const;
    /*! Converts current string to 32-bit unsigned integer.
     *  @param  error Optional placeholder for error flag.
     *  @return Converted value. In case of an error, this value should not be used.
     *  @note If optional error placeholder is given and no error occurs during the conversion, its value is not modifed.
     */
    u32 toUInt(bool* error = NULL) const;
    /*! Converts current string to 64-bit signed integer.
     *  @param  error Optional placeholder for error flag.
     *  @return Converted value. In case of an error, this value should not be used.
     *  @note If optional error placeholder is given and no error occurs during the conversion, its value is not modifed.
     */
    s64 toInt64(bool* error = NULL) const;
    /*! Converts current string to 64-bit unsigned integer.
     *  @param  error Optional placeholder for error flag.
     *  @return Converted value. In case of an error, this value should not be used.
     *  @note If optional error placeholder is given and no error occurs during the conversion, its value is not modifed.
     */
    u64 toUInt64(bool* error = NULL) const;
    /*! Converts current string to boolean.
     *  @param  error Optional placeholder for error flag.
     *  @return Converted value. In case of an error, this value should not be used.
     *  @note If optional error placeholder is given and no error occurs during the conversion, its value is not modifed.
     */
    bool toBool(bool* error = NULL) const;
    /*! Converts current string to 32-bit float value.
     *  @param  error Optional placeholder for error flag.
     *  @return Converted value. In case of an error, this value should not be used.
     *  @note If optional error placeholder is given and no error occurs during the conversion, its value is not modifed.
     */
    float32 toFloat(bool* error = NULL) const;
    /*! Converts current string to 64-bit float value.
     *  @param  error Optional placeholder for error flag.
     *  @return Converted value. In case of an error, this value should not be used.
     *  @note If optional error placeholder is given and no error occurs during the conversion, its value is not modifed.
     */
    float64 toFloat64(bool* error = NULL) const;

    /*! Splits the string into array of substrings whenever seperator occurs. */
    // TAGE - move to StingUtils ? This will remove dependency to DynamicArray
    StringArray split(const String& separator) const;
    /*! Returns a string that has whitespace removed from the start and the end. */
    String trimmed() const;

    /*! Replaces portion of the string with another.
     *  @param  position  0-based index position where replacement should start in current string.
     *  @param  length    Number of characters to replace in current string.
     *  @param  string    String to insert as a replacement.
     *  @return Reference to self.
     */
    String& replace(s32 position, s32 length, const String& string);
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

  private:

    /*! Underlying container. */
    std::string m_value;
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
inline String operator + (const String& left, const String& right)
{
  String string;
  string += left;
  string += right;

  return string;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline bool operator==(const char* left, const String& right)
{
  return (right == left);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline bool operator!=(const char* left, const String& right)
{
  return (right != left);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_STRING_H
