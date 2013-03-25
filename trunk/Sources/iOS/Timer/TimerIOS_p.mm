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
  l_nanoToMicro = 1e3 * info.numer / info.denom;
  l_nanoToMili  = 1e6 * info.numer / info.denom;

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