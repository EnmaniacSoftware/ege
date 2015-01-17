#ifndef EGE_CORE_ATOMIC_ATOMICOPERATIONS_H
#define EGE_CORE_ATOMIC_ATOMICOPERATIONS_H

/*! Platform dependent atomic operation primitives. */

#include "Core/Platform.h"
#include "EGETypes.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Atomically increments the value.
 *  @param  value Value to increment.
 *  @return New value.
 *  @note This uses ordered memory model.
 */
s32 egeAtomicIncrement(volatile s32& value);

/*! Atomically decrements the value.
 *  @param  value Value to increment.
 *  @return New value.
 *  @note This uses ordered memory model.
 */
s32 egeAtomicDecrement(volatile s32& value);

/*! Atomically adds one value to another.
 *  @param  value     Value to be modified.
 *  @param  addValue  Value to be added.
 *  @return Resulting value.
 *  @note This uses ordered memory model.
 */
s32 egeAtomicAdd(volatile s32& value, s32 addValue);

/*! Atomically compares and sets new value depending on the outcome of comparison.
 *  @param  value         Value to test and modify.
 *  @param  compareValue  Value to compare to.
 *  @param  newValue      New value to be set if previous two are equal.
 *  @return Initial value.
 *  @note This uses ordered memory model.
 */
s32 egeAtomicCompareAndSet(volatile s32& value, s32 compareValue, s32 newValue);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_ATOMIC_ATOMICOPERATIONS_H