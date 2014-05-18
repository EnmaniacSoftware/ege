#include "Core/String/StringUtils.h"
#include <stdlib.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 StringUtils::ToInt(const char* text, bool* error)
{
  char* end;

  s32 value = static_cast<s32>(strtol(text, &end, 10));

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
  char* end;

  s64 value = strtoll(text, &end, 10);

  // check for error
  if (error && (0 != *end))
  {
    *error = true;
  }

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END