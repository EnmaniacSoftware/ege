#include "Core/Timer/Timer.h"
#include "iOS/Timer/TimerIOS_p.h"
#include "EGEMath.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool l_initialized = false;
static double l_nanoToMicro = 0;
static double l_nanoToMili = 0;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function initializing high performance counter. */
void Initialize()
{
  // get the time base
  mach_timebase_info_data_t info;
  mach_timebase_info(&info);
  
  // calculate conversion factors
  // NOTE: info.numer / info.denom convert mach_absolute_time() value to nanoseconds (10e9)
  l_nanoToMicro = info.numer / (1000.0 * info.denom);
  l_nanoToMili  = l_nanoToMicro / 1000.0;

  // set flag
  l_initialized = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimerPrivate::Reset()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 TimerPrivate::GetMiliseconds()
{
  if ( ! l_initialized)
  {
    Initialize();
  }
  
  return static_cast<s64>(mach_absolute_time() * l_nanoToMili);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 TimerPrivate::GetMicroseconds()
{ 
  if ( ! l_initialized)
  {
    Initialize();
  }

  return static_cast<s64>(mach_absolute_time() * l_nanoToMicro);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool TimerPrivate::IsHighResolution()
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END