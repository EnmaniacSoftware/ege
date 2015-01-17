#include "Core/Atomic/Interface/AtomicOperations.h"
#include <windows.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 egeAtomicIncrement(volatile s32& value)
{
  return static_cast<s32>(InterlockedIncrement(reinterpret_cast<volatile LONG*>(&value)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 egeAtomicDecrement(volatile s32& value)
{
  return static_cast<s32>(InterlockedDecrement(reinterpret_cast<volatile LONG*>(&value)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 egeAtomicAdd(volatile s32& value, s32 addValue)
{
  return static_cast<s32>(InterlockedExchangeAdd(reinterpret_cast<volatile LONG*>(&value), addValue));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 egeAtomicCompareAndSet(volatile s32& value, s32 compareValue, s32 newValue)
{
  return static_cast<s32>(InterlockedCompareExchange(reinterpret_cast<volatile LONG*>(&value), newValue, compareValue));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END