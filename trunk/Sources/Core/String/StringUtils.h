#ifndef EGE_CORE_STRING_STRINGUTILS_H
#define EGE_CORE_STRING_STRINGUTILS_H

#include <EGETypes.h>
#include <EGERect.h>
#include <EGEAngle.h>
#include <EGETime.h>
#include <EGEColor.h>
#include <EGEString.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class StringUtils
{
  public:

    static s32 ToInt(const char* text, bool* error = NULL);
    static s64 ToInt64(const char* text, bool* error = NULL);

    /*! Converts string to color. 
     *  @param  string  String to convert.
     *  @param  error   Error flag placeholder. If not NULL this will hold conversion result.
     *  @return Converted value.
     *  @note Returned value should be only considered valid if no error has been reported during the conversion.
     */
    static Color ToColor(const String& string, bool* error = NULL);
    /*! Converts string to rectangle of floats. 
     *  @param  string  String to convert
     *  @param  error   Error flag placeholder. If not NULL this will hold conversion result.
     *  @return Converted value.
     *  @note Returned value should be only considered valid if no error has been reported during the conversion.
     */
    static Rectf ToRectf(const String& string, bool* error = NULL);
    /*! Converts string to rectangle of signed integers. 
     *  @param  string  String to convert.
     *  @param  error   Error flag placeholder. If not NULL this will hold conversion result.
     *  @return Converted value.
     *  @note Returned value should be only considered valid if no error has been reported during the conversion.
     */
    static Recti ToRecti(const String& string, bool* error = NULL);
    /*! Converts string to 2D vector of floats.
     *  @param  string  String to convert
     *  @param  error   Error flag placeholder. If not NULL this will hold conversion result.
     *  @return Converted value.
     *  @note Returned value should be only considered valid if no error has been reported during the conversion.
     */
    static Vector2f ToVector2f(const String& string, bool* error = NULL);
    /*! Converts string to 2D vector of signed integers. 
     *  @param  string  String to convert.
     *  @param  error   Error flag placeholder. If not NULL this will hold conversion result.
     *  @return Converted value.
     *  @note Returned value should be only considered valid if no error has been reported during the conversion.
     */
    static Vector2i ToVector2i(const String& string, bool* error = NULL);
    /*! Converts string to 3D vector of floats. 
     *  @param  string  String to convert.
     *  @param  error   Error flag placeholder. If not NULL this will hold conversion result.
     *  @return Converted value.
     *  @note Returned value should be only considered valid if no error has been reported during the conversion.
     */
    static Vector3f ToVector3f(const String& string, bool* error = NULL);
    /*! Converts string to 4D vector of floats. 
     *  @param  string  String to convert.
     *  @param  error   Error flag placeholder. If not NULL this will hold conversion result.
     *  @return Converted value.
     *  @note Returned value should be only considered valid if no error has been reported during the conversion.
     */
    static Vector4f ToVector4f(const String& string, bool* error = NULL);
    /*! Converts string to 4D vector of signed integers. 
     *  @param  string  String to convert.
     *  @param  error   Error flag placeholder. If not NULL this will hold conversion result.
     *  @return Converted value.
     *  @note Returned value should be only considered valid if no error has been reported during the conversion.
     */
    static Vector4i ToVector4i(const String& string, bool* error = NULL);
    /*! Converts string to alignments flag.
     *  @param  string  String to convert.
     *  @param  error   Error flag placeholder. If not NULL this will hold conversion result.
     *  @return Converted value.
     *  @note Returned value should be only considered valid if no error has been reported during the conversion.
     */
    static Alignment ToAlignment(const String& string, bool* error = NULL);
    /*! Converts string to angle. 
     *  @param  string  String to convert. This should contain value in degrees (floating-point).
     *  @param  error   Error flag placeholder. If not NULL this will hold conversion result.
     *  @return Converted value.
     *  @note Returned value should be only considered valid if no error has been reported during the conversion.
     */
    static Angle ToAngle(const String& string, bool* error = NULL);
    /*! Converts string to time.
     *  @param  string  String to convert. This should contain value in seconds (floating-point).
     *  @param  error   Error flag placeholder. If not NULL this will hold conversion result.
     *  @return Converted value.
     *  @note Returned value should be only considered valid if no error has been reported during the conversion.
     */
    static Time ToTime(const String& string, bool* error = NULL);
};    
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_STRING_STRINGUTILS_H