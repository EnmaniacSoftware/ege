#ifndef EGE_TYPES_H
#define EGE_TYPES_H

#include <stdlib.h>             // for NULL
#include "Core/Platform.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
enum EGEResult
{
  EGE_SUCCESS = 0,        // all is ok
  EGE_WAIT,               // opertation still in progress, check later
  EGE_ERROR,              // general failure
  EGE_ERROR_NO_MEMORY,
  EGE_ERROR_BAD_PARAM,
  EGE_ERROR_NOT_SUPPORTED,
  EGE_ERROR_IO,
  EGE_ERROR_ALREADY_EXISTS,
  EGE_ERROR_NOT_FOUND
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#if EGE_PLATFORM_WIN32
#include "Win32/TypesWin32.h"
#elif EGE_PLATFORM_AIRPLAY
#include "Airplay/TypesAirplay.h"
#elif EGE_PLATFORM_IOS
#include "iOS/TypesIOS.h"
#endif // EGE_PLATFORM_WIN32
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef wchar_t Char;

#define EGE_TEXT(txt) L##txt
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_TYPES_H