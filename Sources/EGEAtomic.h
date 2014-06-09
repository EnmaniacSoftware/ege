#ifndef EGE_ATOMIC_H
#define EGE_ATOMIC_H

#include "Core/Platform.h"
#include "EGETypes.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void egeAtomicIncrement(volatile u32& value);
void egeAtomicDecrement(volatile u32& value);
/*! Atomically compares and sets new value depending on the outcome of comparison.
 *  @param  value         Value to test and modify.
 *  @param  compareValue  Value to compare to.
 *  @param  newValue      New value to be set if previous two are equal.
 */
void egeAtomicCompareAndSet(volatile u32& value, u32 compareValue, u32 newValue);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_ATOMIC_H