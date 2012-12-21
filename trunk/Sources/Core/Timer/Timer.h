#ifndef EGE_CORE_TIMER_H
#define EGE_CORE_TIMER_H

#include <EGE.h>
#include <EGESignal.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Timer
{
  public:

    Timer();

  public:

    /*! Returns number of miliseconds passed since system start-up. */
    static s64 GetMiliseconds();
    /* Returns number of microseconds since system start-up.
     * @note If less resolution is available only, it is upscaled to microseconds.
     */
    static s64 GetMicroseconds();
    /*! Returns TRUE if timer is high performace timer. */
    static bool IsHighResolution();
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_TIMER_H
