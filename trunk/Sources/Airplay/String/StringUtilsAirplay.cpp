#include "Core/String/StringUtils.h"
#include <stdlib.h>

EGE_NAMESPACE

// TAGE - HACK for Airplay where strtoll function seems to be missing for VC compiler
#ifdef _MSC_VER
extern "C"
{
  int64_t _strtoi64(const char *nptr, char **endptr, int base);
  int64_t strtoll(const char *nptr, char **endptr, int base)
  {
      return _strtoi64(nptr, endptr, base);
  }
}
#endif // _MSC_VER

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
