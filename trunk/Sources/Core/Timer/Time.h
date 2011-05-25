#ifndef EGE_CORE_TIME_H
#define EGE_CORE_TIME_H

#include "EGETypes.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Time
{
  public:

    Time() : m_time(0){}
    Time(s64 microseconds) : m_time(microseconds) {}
    Time(const Time& time) : m_time(time.m_time) {}
    Time(float32 seconds) : m_time((s32)(seconds * 1000000)) {}
   ~Time(){}

    inline Time operator -(const Time& time) const { return Time(m_time - time.m_time); }
    inline bool operator >(const Time& time) const { return m_time > time.m_time; }
    inline bool operator <(const Time& time) const { return m_time < time.m_time; }

    inline void fromMicroseconds(s64 microseconds) { m_time = microseconds; }
    inline void fromMiliseconds(s64 miliseconds) { m_time = miliseconds * 1000; }
    inline void fromSeconds(float32 seconds) { m_time = (s32)(seconds * 1000000); }

    inline s64 microseconds() const { return m_time; }
    inline s64 miliseconds() const { return m_time / 1000; }
    inline float32 seconds() const { return m_time / 1000000.0f; }

  private:

    /*! Time in microseconds, can be negative. */
    s64 m_time;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_TIME_H
