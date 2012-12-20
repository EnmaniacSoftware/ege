#ifndef EGE_CORE_MATH_TWEENER_INTERFACE_H
#define EGE_CORE_MATH_TWEENER_INTERFACE_H

/** Base interface for tweener object. Tweeners are responsible for mapping a value into another depending on the underlying formulae. 
 */

#include <EGE.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ITweener
{
  public:

    ITweener() {}
    virtual ~ITweener() {}

    /*! Calculates eased-in value for a given normalized time. 
     *  @param time Normalized time for which ease value is to be computed.
     *  @return  Returns calculated ease value.
     */
    virtual float32 easeIn(float32 time) = 0;
    /*! Calculates eased-out value for a given normalized time. 
     *  @param time Normalized time for which ease value is to be computed.
     *  @return  Returns calculated ease value.
     */
    virtual float32 easeOut(float32 time) = 0;
    /*! Calculates eased-in-out value for a given normalized time. 
     *  @param time Normalized time for which ease value is to be computed.
     *  @return  Returns calculated ease value.
     */
    virtual float32 easeInOut(float32 time) = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATH_TWEENER_INTERFACE_H