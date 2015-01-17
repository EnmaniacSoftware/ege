#include "Core/Atomic/Interface/AtomicOperations.h"
#include <libkern/OSAtomic.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 egeAtomicIncrement(volatile s32& value)
{
  return OSAtomicIncrement32Barrier(reinterpret_cast<volatile int32_t*>(&value));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 egeAtomicDecrement(volatile s32& value)
{
  return OSAtomicDecrement32Barrier(reinterpret_cast<volatile int32_t*>(&value));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 egeAtomicAdd(volatile s32& value, s32 addValue)
{
  return OSAtomicAdd32Barrier(addValue, reinterpret_cast<volatile int32_t*>(&value));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 egeAtomicCompareAndSet(volatile s32& value, s32 compareValue, s32 newValue)
{
  return OSAtomicCompareAndSwap32Barrier(compareValue, newValue, reinterpret_cast<volatile int32_t*>(&value));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END