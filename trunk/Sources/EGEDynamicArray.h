#ifndef EGE_DYNAMIC_ARRAY_H
#define EGE_DYNAMIC_ARRAY_H

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "Core/Platform.h"

#if EGE_PLATFORM_WIN32 || EGE_PLATFORM_AIRPLAY
#include "Core/Containers/Stl/DynamicArray.h"
#endif // EGE_PLATFORM_WIN32 || EGE_PLATFORM_AIRPLAY

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef DynamicArray<s32> IntArray;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_DYNAMIC_ARRAY_H