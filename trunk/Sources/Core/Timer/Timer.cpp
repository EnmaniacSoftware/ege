#include "Core/Timer/Timer.h"

#ifdef EGE_PLATFORM_WIN32
#include "Win32/Timer/TimerWin32_p.h"
#elif EGE_PLATFORM_AIRPLAY
#include "Airplay/Timer/TimerAirplay_p.h"
#endif

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Timer::Timer()
{
  TimerPrivate::Reset();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns number of miliseconds passed since system start-up. */
s64 Timer::GetMiliseconds()
{
  return TimerPrivate::GetMiliseconds();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns number of microseconds since system start-up.
    @note If less resolution is available only, it is upscaled to microseconds.
 */
s64 Timer::GetMicroseconds()
{
  return TimerPrivate::GetMicroseconds();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if timer is high performace timer. */
bool Timer::IsHighResolution()
{
  return TimerPrivate::IsHighResolution();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END