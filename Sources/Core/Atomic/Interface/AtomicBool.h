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

    /*! Current value. */
    volatile bool m_value;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_ATOMIC_ATOMICBOOL_H