#ifndef EGE_CORE_STRING_UTILS_H
#define EGE_CORE_STRING_UTILS_H

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class StringUtils
{
  public:

    static float ToFLOAT(const std::string& strText);
    static s32   ToS32(const std::string& strText);
    static u32   ToU32(const std::string& strText);
    static u16   ToU16(const std::string& strText);
    static s16   ToS16(const std::string& strText);
    static u8    ToU8(const std::string& strText);
    static s8    ToS8(const std::string& strText);
    static bool  ToBOOL(const std::string& strText);

    static std::string ToLowercase(const std::string& strText);

    static float ToFLOAT(const std::wstring& strText);
    static s32   ToS32(const std::wstring& strText);
    static u32   ToU32(const std::wstring& strText);
    static u16   ToU16(const std::wstring& strText);
    static s16   ToS16(const std::wstring& strText);
    static u8    ToU8(const std::wstring& strText);
    static s8    ToS8(const std::wstring& strText);
    static bool  ToBOOL(const std::wstring& strText);

    static std::wstring ToLowercase(const std::wstring& strText);
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_STRING_UTILS_H