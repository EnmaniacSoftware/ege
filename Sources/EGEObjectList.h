#ifndef EGE_OBJECTLIST_H
#define EGE_OBJECTLIST_H

#include "Core/Platform.h"
#include "Core/Memory/Object.h"
#include "EGEList.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Object, PObject)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef List<PObject> ObjectList;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_OBJECTLIST_H