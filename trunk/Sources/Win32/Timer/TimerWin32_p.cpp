#include "Core/Timer/Timer.h"
#include "Win32/Timer/TimerWin32_p.h"
#include "EGEMath.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(TimerPrivate)
EGE_DEFINE_DELETE_OPERATORS(TimerPrivate)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#define FREQUENCY_RESAMPLE_RATE 200
#define DISCRAPANCY_THRESHOLD   500

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TimerPrivate::TimerPrivate(Timer* base) : m_base(base), m_isPerformance(false), m_startTick(0), m_procMask(0), m_sysMask(0), m_queryCount(0), m_lastTime(0), 
                                          m_thread(NULL)
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
  m_isPerformance = true;

  // get performance counters data
  if (FALSE == QueryPerformanceFrequency(&m_frequency) || FALSE == QueryPerformanceCounter(&m_startTime))
  {
    // no support for performace counter
    m_isPerformance = false;
  }

  // get reference times
  m_startTick = GetTickCount();

  // reset data
  m_lastTime = 0;

  m_queryCount = 0;

  // get the current affinity
#if _MSC_VER >= 1400 && defined (_M_X64)
  GetProcessAffinityMask(GetCurrentProcess(), reinterpret_cast<PDWORD_PTR>(&m_procMask), reinterpret_cast<PDWORD_PTR>(&m_sysMask));
#else
  GetProcessAffinityMask(GetCurrentProcess(), reinterpret_cast<DWORD*>(&m_procMask), reinterpret_cast<DWORD*>(&m_sysMask));
#endif

  // get thread handle
  m_thread = GetCurrentThread();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns number of miliseconds passed so far. */
s64 TimerPrivate::milliseconds()
{
  // check if no performace counter is available
  if (!isHighResolution())
  {
    return (s64) GetTickCount();
  }

  // get number of ticks
  LONGLONG llTicks = updatePerformanceTimer();

  // convert to milliseconds
  return static_cast<s64>(1000 * llTicks / m_frequency.QuadPart);
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
    return (s64) GetTickCount() * 1000;
  }

  // get number of ticks
  LONGLONG llTicks = updatePerformanceTimer();

  // convert to microseconds
  return static_cast<s64>(1000000 * llTicks / m_frequency.QuadPart);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if timer is high performace timer. */
bool TimerPrivate::isHighResolution() const
{
  return m_isPerformance;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates performance counter. */
LONGLONG TimerPrivate::updatePerformanceTimer()
{
  LARGE_INTEGER sCurTime;

  // query performance counter
  SetThreadAffinityMask(m_thread, 1);
  QueryPerformanceCounter(&sCurTime);
  SetThreadAffinityMask(m_thread, m_procMask);

  // resample the frequency
  if (FREQUENCY_RESAMPLE_RATE == ++m_queryCount)
  {
    m_queryCount = 0;
    QueryPerformanceFrequency(&m_frequency);
  }

  LONGLONG llNewTime = sCurTime.QuadPart - m_startTime.QuadPart;
  
  // scale by 1000 for milliseconds
  u32 uiNewTicks = static_cast<u32>(1000 * llNewTime / m_frequency.QuadPart);

  // detect and compensate for performance counter leaps (surprisingly common, see Microsoft KB: Q274323)
  u32 uiCheck = GetTickCount() - m_startTick;

  s32 iMsecOff = static_cast<s32>(uiNewTicks - uiCheck);

  if (-DISCRAPANCY_THRESHOLD > iMsecOff || DISCRAPANCY_THRESHOLD < iMsecOff)
  {
    // we must keep the timer running forward :)
    LONGLONG llAdjust = Math::Min(iMsecOff * m_frequency.QuadPart / 1000, llNewTime - m_lastTime);

    m_startTime.QuadPart += llAdjust;

    llNewTime -= llAdjust;
  }

  // record last time for adjust
  m_lastTime = llNewTime;

  return llNewTime;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
