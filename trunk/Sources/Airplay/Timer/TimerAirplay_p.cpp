#include "Core/Timer/Timer.h"
#include "Airplay/Timer/TimerAirplay_p.h"
#include "Core/Math/Math.h"
#include <s3e.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Resets timer. */
void TimerPrivate::Reset()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns number of miliseconds passed since system start-up. */
s64 TimerPrivate::GetMiliseconds()
{
  return (s64) s3eTimerGetMs();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns number of microseconds since system start-up.
    @note If less resolution is available only, it is upscaled to microseconds.
 */
s64 TimerPrivate::GetMicroseconds()
{
  return (s64) s3eTimerGetMs() * 1000;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if timer is high performace timer. */
bool TimerPrivate::IsHighResolution()
{
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END