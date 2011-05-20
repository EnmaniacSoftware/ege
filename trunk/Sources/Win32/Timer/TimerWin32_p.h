#ifndef EGE_WIN32_TIMER_PRIVATE_H
#define EGE_WIN32_TIMER_PRIVATE_H

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class TimerPrivate
{
  public:

    TimerPrivate(Timer* base);
   ~TimerPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if timer is high performace timer. */
    bool isHighResolution() const;
    /* Returns number of miliseconds passed so far. */
    s64 milliseconds();
    /** Returns number of microseconds passed so far.
    *
    *   If less resolution is available only, it is upscaled to microseconds.
    */
    s64 microseconds();
    /* Resets timer. */
    void reset(); 

  private:

    /* Updates performance counter. */
    LONGLONG updatePerformanceTimer();

  private:

    /*! Pointer to base object. */
    Timer* m_base;
    /*! TRUE if timer is capable of microsecs. */
    bool m_isPerformance;
    /*! Reference time for GetTickCount. */
    u32 m_startTick;
    /*! Process affinity mask. */
    u32 m_procMask;
    /*! System affinity mask. */
    u32 m_sysMask;
    /*! Number of times performance timer was used. */
    u32 m_queryCount;
    /*! Performance counter last time. */
    LONGLONG m_lastTime;
    /*! Reference time for performace counter. */
    LARGE_INTEGER m_startTime;
    /*! Performance counter frequency. */
    LARGE_INTEGER m_frequency;
    /*! Handle to thread timer is running on. */
    HANDLE m_thread;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_WIN32_TIMER_PRIVATE_H
