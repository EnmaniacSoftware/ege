#include "EGEAtomic.h"
#include <libkern/OSAtomic.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void egeAtomicIncrement(volatile u32& value)
{
  OSAtomicIncrement32Barrier(reinterpret_cast<volatile int32_t*>(&value));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void egeAtomicDecrement(volatile u32& value)
{
  OSAtomicDecrement32Barrier(reinterpret_cast<volatile int32_t*>(&value));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END