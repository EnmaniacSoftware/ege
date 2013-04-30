#ifndef EGE_MEMORY_H
#define EGE_MEMORY_H

#include "EGETypes.h"
#include "Core/Memory/Object.h"
#include "Core/Memory/SmartPointer.h"
#include "Core/Memory/MemoryManager.h"
#include "Core/Memory/Memory.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Object, PObject)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T, class U>
inline T ege_cast(SmartPointer<U> object)
{
  return static_cast<T>(object.object());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T, class U>
inline T ege_pcast(SmartPointer<U> object)
{
  return T(object);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_EVENT_H