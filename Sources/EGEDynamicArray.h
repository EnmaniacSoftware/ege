#ifndef EGE_DYNAMIC_ARRAY_H
#define EGE_DYNAMIC_ARRAY_H

#include "EGETypes.h"

#if EGE_CONTAINERS_STL
  #include "Core/Containers/Interface/STL/DynamicArray.h"
#endif // EGE_CONTAINERS_STL

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef DynamicArray<s32>     IntArray;
typedef DynamicArray<float32> FloatArray;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_DYNAMIC_ARRAY_H