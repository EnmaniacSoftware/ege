#ifndef EGE_CORE_STRING_TEXTSTREAM_H
#define EGE_CORE_STRING_TEXTSTREAM_H

/*! This class provides a convenient interface for reading and writing text.
 */

#include "EGE.h"
#include "EGEString.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IODevice;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class TextStream
{
  public:

    explicit TextStream(IODevice* device);
   ~TextStream();

  operators:

    TextStream& operator << (u8 value);
    TextStream& operator << (s8 value);
    TextStream& operator << (u16 value);
    TextStream& operator << (s16 value);
    TextStream& operator << (u32 value);
    TextStream& operator << (s32 value);
    TextStream& operator << (u64 value);
    TextStream& operator << (s64 value);
    TextStream& operator << (bool value);
    TextStream& operator << (float32 value);
    TextStream& operator << (float64 value);
    TextStream& operator << (const char* value);
    TextStream& operator << (const String& value);

    TextStream& operator >> (u8& value);
    TextStream& operator >> (s8& value);
    TextStream& operator >> (u16& value);
    TextStream& operator >> (s16& value);
    TextStream& operator >> (u32& value);
    TextStream& operator >> (s32& value);
    TextStream& operator >> (u64& value);
    TextStream& operator >> (s64& value);
    TextStream& operator >> (bool& value);
    TextStream& operator >> (float32& value);
    TextStream& operator >> (float64& value);

    /*! Reads next word and adds NULL terminator. */
    TextStream& operator >> (char* value);
    /*! Reads next word. */
    TextStream& operator >> (String& value);

  public:

    /*! Returns status. */
    bool isGood() const;

    /*! Sets float point precision.
     *  @param  precision Desired precision.
     */
    void setFloatPrecision(s32 precision);
    /*! Returns float point precision. */
    s32 floatPrecision() const;

  private:

    /*! Device. */
    IODevice* m_device;
    /*! Status. */
    bool m_good;
    /*! Float precision. 
     *  @note Number of fraction digits.
     */
    s32 m_floatPrecision;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_STRING_TEXTSTREAM_H
