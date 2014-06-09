#include "EGEAtomic.h"
#include <windows.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void egeAtomicIncrement(volatile u32& value)
{
  InterlockedIncrement(&value);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void egeAtomicDecrement(volatile u32& value)
{
  InterlockedDecrement(&value);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void egeAtomicCompareAndSet(volatile u32& value, u32 compareValue, u32 newValue)
{
  InterlockedCompareExchangeAcquire(&value, compareValue, newValue);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END