#ifndef EGE_WIN32_TIMER_PRIVATE_H
#define EGE_WIN32_TIMER_PRIVATE_H

#include <EGE.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class TimerPrivate
{
  public:

    /* Returns number of miliseconds passed since system start-up. */
    static s64 GetMiliseconds();
    /* Returns number of microseconds since system start-up.
       @note If less resolution is available only, it is upscaled to microseconds.
     */
    static s64 GetMicroseconds();
    /* Returns TRUE if timer is high performace timer. */
    static bool IsHighResolution();

    /* Resets timer. */
    static void Reset(); 

  private:

    /* Updates performance counter. */
    static LONGLONG UpdatePerformanceTimer();

  private:

    /*! TRUE if timer is capable of microsecs. */
    static bool m_isPerformance;
    /*! Reference time for GetTickCount. */
    static u32 m_startTick;
    /*! Process affinity mask. */
    static u32 m_procMask;
    /*! System affinity mask. */
    static u32 m_sysMask;
    /*! Number of times performance timer was used. */
    static u32 m_queryCount;
    /*! Performance counter last time. */
    static LONGLONG m_lastTime;
    /*! Reference time for performace counter. */
    static LARGE_INTEGER m_startTime;
    /*! Performance counter frequency. */
    static LARGE_INTEGER m_frequency;
    /*! Handle to thread timer is running on. */
    static HANDLE m_thread;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_WIN32_TIMER_PRIVATE_H
