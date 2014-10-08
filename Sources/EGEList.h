#ifndef EGE_LIST_H
#define EGE_LIST_H

#include "Core/Platform.h"

#if EGE_CONTAINERS_STL
  #include "Core/Containers/Interface/STL/List.h"
#endif // EGE_CONTAINERS_STL

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef List<s32> IntList;
typedef List<float32> FloatList;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_LIST_H