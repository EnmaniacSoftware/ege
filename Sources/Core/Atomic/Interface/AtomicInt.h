#ifndef EGE_CORE_ATOMIC_ATOMICINT_H
#define EGE_CORE_ATOMIC_ATOMICINT_H

/** Class provides platform-independent atomic operations on integers.
 */

#include "EGETypes.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AtomicInt
{
  public:

    AtomicInt(s32 value = 0);

    /*! Returns current value.
     *  @note This uses ordered memory model.
     */
    s32 load();

    /*! Increments current value. *
     *  @return New value.
     *  @note This uses ordered memory model. 
     */
    s32 increment();
    /*! Decrements current value. *
     *  @return New value.
     *  @note This uses ordered memory model. 
     */
    s32 decrement();

    /*! Tests and sets new value if test succeeds.
     *  @param  expected  Value to be tested agaist.
     *  @param  newValue  New Value to be set if test is successful.
     *  @return Initial value.
     *  @note This uses ordered memory model. 
     */
    s32 testAndSet(s32 expected, s32 newValue);

  private:

    /*! Current value. */
    volatile s32 m_value;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_ATOMIC_ATOMICINT_H