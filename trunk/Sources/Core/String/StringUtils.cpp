//#include "Core/EGEngine.h"
#include "Core/String/StringUtils.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

float StringUtils::ToFLOAT(const std::string& strText)
{
  //istringstream str(strText);

  //float ret;

  //str >> ret;

  //strtol(

  return 0;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

s32 StringUtils::ToS32(const std::string& strText)
{
  s32 iValue;

  iValue = strtol(strText.c_str(), NULL, 10);

  return iValue;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

u32 StringUtils::ToU32(const std::string& strText)
{
  u32 uiValue;

  uiValue = strtoul(strText.c_str(), NULL, 10);

  return uiValue;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

u16 StringUtils::ToU16(const std::string& strText)
{
  return static_cast<u16>(StringUtils::ToU32(strText));
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

s16 StringUtils::ToS16(const std::string& strText)
{
  return static_cast<s16>(StringUtils::ToS32(strText));
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

u8 StringUtils::ToU8(const std::string& strText)
{
  return static_cast<u8>(StringUtils::ToU32(strText));
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

s8 StringUtils::ToS8(const std::string& strText)
{
  return static_cast<s8>(StringUtils::ToS32(strText));
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

bool StringUtils::ToBOOL(const std::string& strText)
{
  // transform to lowercase
  std::string strTmp = StringUtils::ToLowercase(strText);

  return std::string::npos != strTmp.find("true");
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

std::string StringUtils::ToLowercase(const std::string& strText)
{
  std::string strTmp = strText;

  // transform to lowercase
  std::transform(strTmp.begin(), strTmp.end(), strTmp.begin(), std::tolower);

  return strTmp;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

float StringUtils::ToFLOAT(const std::wstring& strText)
{
  //istringstream str(strText);

  //float ret;

  //str >> ret;

  //strtol(

  return 0;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

s32 StringUtils::ToS32(const std::wstring& strText)
{
  s32 iValue;

  iValue = wcstol(strText.c_str(), NULL, 10);

  return iValue;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

u32 StringUtils::ToU32(const std::wstring& strText)
{
  u32 uiValue;

  uiValue = wcstoul(strText.c_str(), NULL, 10);

  return uiValue;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

u16 StringUtils::ToU16(const std::wstring& strText)
{
  return static_cast<u16>(StringUtils::ToU32(strText));
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

s16 StringUtils::ToS16(const std::wstring& strText)
{
  return static_cast<s16>(StringUtils::ToS32(strText));
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

u8 StringUtils::ToU8(const std::wstring& strText)
{
  return static_cast<u8>(StringUtils::ToU32(strText));
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

s8 StringUtils::ToS8(const std::wstring& strText)
{
  return static_cast<s8>(StringUtils::ToS32(strText));
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

bool StringUtils::ToBOOL(const std::wstring& strText)
{
  // transform to lowercase
  std::wstring strTmp = StringUtils::ToLowercase(strText);

  return std::wstring::npos != strTmp.find(L"true");
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

std::wstring StringUtils::ToLowercase(const std::wstring& strText)
{
  std::wstring strTmp = strText;

  // transform to lowercase
  std::transform(strTmp.begin(), strTmp.end(), strTmp.begin(), std::tolower);

  return strTmp;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
