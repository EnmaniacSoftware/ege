#include "Core/Timer/Timer.h"
#include "Airplay/Timer/TimerAirplay_p.h"
#include "Core/Math/Math.h"
#include "s3e.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(TimerPrivate)
EGE_DEFINE_DELETE_OPERATORS(TimerPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TimerPrivate::TimerPrivate(Timer* base) : m_base(base), 
                                          m_isPerformance(false)
{
  reset();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TimerPrivate::~TimerPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Resets timer. */
void TimerPrivate::reset()
{
  m_isPerformance = false;

  // get performance counters data
  //if (-1 == s3eTimerGetUSTNanoseconds())
  //{
  //  // no support for performace counter
  //  m_isPerformance = false;
  //}

  //// get reference times
  //m_highPerfLastTimeMeasurement = s3eTimerGetUSTNanoseconds();

  //// reset data
  //m_llLastTime = 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns number of miliseconds passed so far. */
s64 TimerPrivate::milliseconds()
{
  // check if no performace counter is available
  if (!isHighResolution())
  {
    return (s64) s3eTimerGetMs();
  }

  // get number of ticks
  //s64 ticks = updatePerformanceTimer();

  // convert to milliseconds
  return 0;//static_cast<s64>(ticks / 1000000);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/** Returns number of microseconds passed so far.
*
*   If less resolution is available only, it is upscaled to microseconds.
*/
s64 TimerPrivate::microseconds()
{
  // check if no performace counter is available
  if (!isHighResolution())
  {
    return (s64) s3eTimerGetMs() * 1000;
  }

  //// get number of ticks
  //LONGLONG llTicks = updatePerformanceTimer();

  //// convert to microseconds
  //return static_cast<s64>(1000000 * llTicks / m_sFrequency.QuadPart);
  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if timer is high performace timer. */
bool TimerPrivate::isHighResolution() const
{
  return m_isPerformance;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates performance counter. */
s64 TimerPrivate::updatePerformanceTimer()
{
  return 0;
  /*
  s64 currentTime = s3eTimerGetUSTNanoseconds();
  s64 timeDiff;

  // check if we wrap around
  if (currentTime < m_highPerfLastTimeMeasurement)
  {
    timeDiff = currentTime + (INT64_MAX - m_highPerfLastTimeMeasurement);
  }
  else
  {
    timeDiff = currentTime - m_highPerfLastTimeMeasurement;
  }  

  // record last time for adjust
  m_highPerfLastTimeMeasurement = currentTime;

  return currentTime;*/
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END