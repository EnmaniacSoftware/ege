#include "Core/Timer/Interface/Timer.h"
#include "EGEMath.h"
#include <mach/mach_time.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool l_Initialized = false;

static double l_NanoToMicro = 0;
static double l_NanoToMili  = 0;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function initializing high performance counter. */
static void Initialize()
{
  // get the time base
  mach_timebase_info_data_t info;
  mach_timebase_info(&info);
  
  // calculate conversion factors
  // NOTE: info.numer / info.denom convert mach_absolute_time() value to nanoseconds (10e9)
  l_NanoToMicro = info.numer / (1000.0 * info.denom);
  l_NanoToMili  = l_NanoToMicro / 1000.0;

  // set flag
  l_Initialized = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 Timer::GetMiliseconds()
{
  if ( ! l_Initialized)
  {
    Initialize();
  }
  
  return static_cast<s64>(mach_absolute_time() * l_NanoToMili);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 Timer::GetMicroseconds()
{ 
  if ( ! l_Initialized)
  {
    Initialize();
  }

  return static_cast<s64>(mach_absolute_time() * l_NanoToMicro);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Timer::IsHighResolution()
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
