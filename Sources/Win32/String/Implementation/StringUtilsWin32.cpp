#include "Core/String/Interface/StringUtils.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 StringUtils::ToInt(const char* text, bool* error)
{
  char* end;

  s32 value = strtol(text, &end, 10);

  // check for error
  if (error && (0 != *end))
  {
    *error = true;
  }

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 StringUtils::ToInt64(const char* text, bool* error)
{
  s64 value = _atoi64(text);
  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END