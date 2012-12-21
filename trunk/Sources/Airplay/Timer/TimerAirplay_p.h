#ifndef EGE_AIRPLAY_TIMER_PRIVATE_H
#define EGE_AIRPLAY_TIMER_PRIVATE_H

#include <EGE.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class TimerPrivate
{
  public:

    /*! Returns number of miliseconds passed since system start-up. */
    static s64 GetMiliseconds();
    /*! Returns number of microseconds since system start-up.
     *  @note If less resolution is available only, it is upscaled to microseconds.
     */
    static s64 GetMicroseconds();
    /*! Returns TRUE if timer is high performace timer. */
    static bool IsHighResolution();

    /*! Resets timer. */
    static void Reset(); 
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_AIRPLAY_TIMER_PRIVATE_H
