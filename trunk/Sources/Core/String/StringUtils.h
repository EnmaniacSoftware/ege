#ifndef EGE_CORE_STRINGUTILS_H
#define EGE_CORE_STRINGUTILS_H

#include <EGETypes.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class StringUtils
{
  public:

    static s32 ToInt(const char* text, bool* error = NULL);
    static s64 ToInt64(const char* text, bool* error = NULL);
};    
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_STRINGUTILS_H