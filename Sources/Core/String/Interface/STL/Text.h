#ifndef EGE_CORE_TEXT_H
#define EGE_CORE_TEXT_H

#include "EGETypes.h"
#include <string>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Text
{
  public:
    
    Text();
    Text(const Text& other);
    Text(const std::wstring& other);

    // TAGE - check
    Text(const Char* string);
    // TAGE - check
    Text(const Char* string, s32 length);
    // TAGE - check
    Text(const char* string);
   ~Text();

  operators:

    Text& operator=(const Text& other);
   	
    Text& operator+=(const Text& other);

    bool operator==(const Text& other) const;
    
    bool operator!=(const Text& other) const;

  public:

    /*! Create new object from formatted text. */
    static Text Format(const char* text, ...);

  public:

    /*! Returns length of the string (in bytes). 
     *  @note Returned value does not contain NULL termination character.
     */
    s32 length() const;
    /*! Clears the string. */
    void clear();
    /*! Returns TRUE if string is empty. */
    bool isEmpty() const;

    /*! Returns character at a given index.
     *  @param  index   0-based index of character.
     *  @return Character found.
     */
    Char at(s32 index) const;

    /*! Returns pointer to an array containing text data. 
     *  @note Returned array is NULL terminated.
     */
    const Char* data() const;

    /*! Returns the index position of the first occurrence of the string in this string, searching forward from given index position.
     *  @param  string    String to look for.
     *  @param  position  Index position the search is to be started from.
     *  @return 0-based index of the begining of a given string within the current one or negative value if string is not found.
     */
    s32 indexOf(const Text& string, s32 position = 0) const;

    /*! Creates formatted text. */
    void format(const char* text, ...);
    /*! Converts self to lower-case. */
    Text& toLower();
    /*! Converts self to upper-case. */
    Text& toUpper();

    /*! Returns copy of the current string with lowest arg marker replaced with a given string. */
    //Text arg(const String& string) const;
    /*! Returns copy of the current string with lowest arg marker replaced with a given string. */
    Text arg(const Text& string) const;
    /*! Returns copy of the current string with lowest arg marker replaced with a given integer value. */
    Text arg(s32 value) const;
    /*! Returns copy of the current string with lowest arg marker replaced with a given float value. */
    Text arg(float32 value) const;

    /*! Replaces portion of the string with another.
     *  @param  position  0-based index position where replacement should start in current string.
     *  @param  length    Number of characters to replace in current string.
     *  @param  string    String to insert as a replacement.
     *  @return Reference to self.
     */
    Text& replace(s32 position, s32 length, const Text& string);
    /*! Replaces all occurences of the string with another string. 
     *  @param  before String to be replaced.
     *  @param  after  String to be placed.
     *  @return Reference to self.
     */
    Text& replaceAll(const Text& before, const Text& after);

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

    /*! Converts from given UTF-8 string into UTF-16. */
    //bool fromString(const String& string);
    /*! Finds data about arg escape of the lowest sequence number. */
    ArgEscapeData findArgEscapes() const;
    /*! Replaces args with given string. */
    void replaceArgEscapes(Text& out, const Text& arg, ArgEscapeData& argData) const;

  private:

    /*! Underlying container. */
    std::wstring m_value;
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