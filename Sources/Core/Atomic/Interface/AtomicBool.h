#ifndef EGE_CORE_ATOMIC_ATOMICBOOL_H
#define EGE_CORE_ATOMIC_ATOMICBOOL_H

/** Class provides platform-independent atomic operations on booleans.
 */

#include "EGETypes.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AtomicBool
{
  public:

    explicit AtomicBool(bool value = false);
    explicit AtomicBool(const AtomicBool& other);

    /*! Returns current value.
     *  @note This uses ordered memory model.
     */
    bool load();

    /*! Sets new value.
     *  @param  value  Value to be set.
     *  @return Initial value.
     *  @note This uses ordered memory model. 
     */
    bool store(bool value);

  private:

    /*! Current value. 
     *  @note Internal implementation is a 32-bit signed integer.
     */
    volatile s32 m_value;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_ATOMIC_ATOMICBOOL_H