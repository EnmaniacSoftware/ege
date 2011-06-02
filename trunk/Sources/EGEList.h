#ifndef EGE_LIST_H
#define EGE_LIST_H

#include "EGE.h"

#if EGE_PLATFORM_WIN32 || EGE_PLATFORM_AIRPLAY
#include "Core/Containers/Stl/List.h"
#endif // EGE_PLATFORM_WIN32 || EGE_PLATFORM_AIRPLAY

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef EGEList<s32> EGEIntList;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_LIST_H