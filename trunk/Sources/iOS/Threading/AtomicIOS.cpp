#include "EGEAtomic.h"
#include <libkern/OSAtomic.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void egeAtomicIncrement(volatile u32& value)
{
  volatile int32_t val = static_cast<int32_t>(value);
  value = static_cast<u32>(OSAtomicIncrement32(&val));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void egeAtomicDecrement(volatile u32& value)
{
  volatile int32_t val = static_cast<int32_t>(value);
  value = static_cast<u32>(OSAtomicDecrement32(&val));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END