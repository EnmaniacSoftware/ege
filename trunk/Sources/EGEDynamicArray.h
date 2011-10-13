#ifndef EGE_DYNAMIC_ARRAY_H
#define EGE_DYNAMIC_ARRAY_H

#include <EGETypes.h>

#if EGE_PLATFORM_WIN32 || EGE_PLATFORM_AIRPLAY
#include "Core/Containers/Stl/DynamicArray.h"
#endif // EGE_PLATFORM_WIN32 || EGE_PLATFORM_AIRPLAY

#include <EGEString.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef DynamicArray<s32>     IntArray;
typedef DynamicArray<float32> FloatArray;
typedef DynamicArray<String>  StringArray;
typedef DynamicArray<Text>    TextArray;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_DYNAMIC_ARRAY_H