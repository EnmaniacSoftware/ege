#ifndef EGE_AIRPLAY_TIMER_PRIVATE_H
#define EGE_AIRPLAY_TIMER_PRIVATE_H

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
    s64 updatePerformanceTimer();

  private:

    /*! Pointer to base object. */
    Timer* m_base;
    /*! TRUE if timer is capable of microsecs. */
    bool m_isPerformance;
    /*! Last high-resolution measurment taken. */
    s64 m_highPerfLastTimeMeasurement;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_AIRPLAY_TIMER_PRIVATE_H
