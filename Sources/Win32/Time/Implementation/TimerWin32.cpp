#include "Core/Time/Interface/Timer.h"
#include "EGEMath.h"
#include <windows.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool l_IsHighPerformance         = false;
static bool l_WasHighPerformanceChecked = false;

static u32 l_HightPerformaceStartTick                       = 0;
static u32 l_HighPerformanceCounterFrequenceResampleCounter = 0;
static u32 l_ProcMask                                       = 0;
static u32 l_SysMask                                        = 0;

static LONGLONG l_LastTime = 0;

static HANDLE l_ThreadHandle = NULL;

static LARGE_INTEGER l_HighPerformanceCounterStartTime;
static LARGE_INTEGER l_HighPerformanceCounterFrequency;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const s32 KFrequencyResampleRate = 200;
static const s32 KDiscrepancyThreshold  = 500;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function detecting high performance timer availability. */
static void DetectHighPerformance()
{
  l_IsHighPerformance = true;

  // get performance counters data
  if ((FALSE == QueryPerformanceFrequency(&l_HighPerformanceCounterFrequency)) || (FALSE == QueryPerformanceCounter(&l_HighPerformanceCounterStartTime)))
  {
    // no support for performace counter
    l_IsHighPerformance = false;
  }

  // get reference times
  l_HightPerformaceStartTick = GetTickCount();

  // reset data
  l_LastTime    = 0;
  l_HighPerformanceCounterFrequenceResampleCounter  = 0;

  // get the current affinity
#if _MSC_VER >= 1400 && defined (_M_X64)
  GetProcessAffinityMask(GetCurrentProcess(), reinterpret_cast<PDWORD_PTR>(&l_ProcMask), reinterpret_cast<PDWORD_PTR>(&l_SysMask));
#else
  GetProcessAffinityMask(GetCurrentProcess(), reinterpret_cast<DWORD*>(&l_ProcMask), reinterpret_cast<DWORD*>(&l_SysMask));
#endif

  // get thread handle
  l_ThreadHandle = GetCurrentThread();

  // make check was done
  l_WasHighPerformanceChecked = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function updating performance timer for drifts. 
 *  @return High performance timer value.
 */
static LONGLONG UpdatePerformanceTimer()
{
  LARGE_INTEGER currentTime;

  // query performance counter
  SetThreadAffinityMask(l_ThreadHandle, 1);
  QueryPerformanceCounter(&currentTime);
  SetThreadAffinityMask(l_ThreadHandle, l_ProcMask);

  // resample the frequency
  if (KFrequencyResampleRate == ++l_HighPerformanceCounterFrequenceResampleCounter)
  {
    // reset resample counter
    l_HighPerformanceCounterFrequenceResampleCounter = 0;

    // retrieve counter frequency
    QueryPerformanceFrequency(&l_HighPerformanceCounterFrequency);
  }

  // calculate time differance with high performace counter
  LONGLONG hpTimeDifference = currentTime.QuadPart - l_HighPerformanceCounterStartTime.QuadPart;
  
  // convert high performance time difference to miliseconds
  const u32 hpTimeDifferenceInLowPrecision = static_cast<u32>(1000 * hpTimeDifference / l_HighPerformanceCounterFrequency.QuadPart);

  // calculate time difference with low performance counter
  u32 lpTimeDifference = GetTickCount() - l_HightPerformaceStartTick;

  // detect and compensate for performance counter leaps (surprisingly common, see Microsoft KB: Q274323)
  const s32 hpDrift = static_cast<s32>(static_cast<s64>(hpTimeDifferenceInLowPrecision) - static_cast<s64>(lpTimeDifference));
  if ((-KDiscrepancyThreshold > hpDrift) || (KDiscrepancyThreshold < hpDrift))
  {
    // we must keep the timer running forward :)
    const LONGLONG hpAdjustment = Math::Min(hpDrift * l_HighPerformanceCounterFrequency.QuadPart / 1000, hpTimeDifference - l_LastTime);

    // update calculation data
    l_HighPerformanceCounterStartTime.QuadPart += hpAdjustment;
    hpTimeDifference -= hpAdjustment;
  }

  // record last time for adjustment
  l_LastTime = hpTimeDifference;

  return hpTimeDifference;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 Timer::GetMiliseconds()
{
  // check if no performace counter is available
  if ( ! IsHighResolution())
  {
    return static_cast<s64>(GetTickCount());
  }

  // get number of ticks
  const LONGLONG ticks = UpdatePerformanceTimer();

  // convert to milliseconds
  return static_cast<s64>(1000 * ticks / l_HighPerformanceCounterFrequency.QuadPart);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s64 Timer::GetMicroseconds()
{
  // check if no performace counter is available
  if ( ! IsHighResolution())
  {
    return static_cast<s64>(GetTickCount());
  }

  // get number of ticks
  const LONGLONG ticks = UpdatePerformanceTimer();

  // convert to microseconds
  return static_cast<s64>(1000000 * ticks / l_HighPerformanceCounterFrequency.QuadPart);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Timer::IsHighResolution()
{
  // check if high performance timer availability was NOT checked yet
  if ( ! l_WasHighPerformanceChecked)
  {
    // detect high performance availability
    DetectHighPerformance();
  }

  return l_IsHighPerformance;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
