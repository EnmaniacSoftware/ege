#ifndef EGE_CORE_TIME_H
#define EGE_CORE_TIME_H

/** 
  This class encapsulates time duration.
*/

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include <EGETypes.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Time
{
  public:

    Time() : m_time(0) {}
    Time(s64 microseconds) : m_time(microseconds) {}
    Time(const Time& time) : m_time(time.m_time) {}
    Time(float32 seconds) : m_time(static_cast<s64>(seconds * 1000000)) {}

    inline bool operator > (const Time& time) const { return m_time > time.m_time; }
    inline bool operator >= (const Time& time) const { return m_time >= time.m_time; }
    inline bool operator < (const Time& time) const { return m_time < time.m_time; }
    inline bool operator <= (const Time& time) const { return m_time <= time.m_time; }
    inline bool operator == (const Time& time) { return m_time == time.m_time; }
    inline bool operator != (const Time& time) { return m_time != time.m_time; }

    inline Time& operator += (const Time& time) { m_time += time.m_time; return *this; }
    inline Time& operator -= (const Time& time) { m_time -= time.m_time; return *this; }

    /*! Sets time from given microseconds. */
    inline void fromMicroseconds(s64 microseconds) { m_time = microseconds; }
    /*! Sets time from given miliseconds. */
    inline void fromMiliseconds(s64 miliseconds) { m_time = miliseconds * 1000LL; }
    /*! Sets time from given seconds. */
    inline void fromSeconds(float32 seconds) { m_time = static_cast<s64>(seconds) * 1000000LL; }

    /*! Returns time duration in microseconds. */
    inline s64 microseconds() const { return m_time; }
    /*! Returns time duration in miliseconds. */
    inline s64 miliseconds() const { return m_time / 1000; }
    /*! Returns time duration in seconds (with fractions). */
    inline float32 seconds() const { return m_time / 1000000.0f; }
    /*! Returns time duration in minutes (with fractions). */
    inline float32 minutes() const { return m_time / 60000000.0f; }

  public:

    /*! Returns Time object from given seconds. */
    static Time FromSeconds(float32 seconds) { Time time(seconds); return time; }

  private:

    /*! Time in microseconds, can be negative. */
    s64 m_time;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline Time operator+(const Time& left, const Time& right)
{
  return Time(left.microseconds() + right.microseconds());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline Time operator-(const Time& left, const Time& right)
{
  return Time(left.microseconds() - right.microseconds());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline Time operator/(const Time& left, float32 factor)
{
  return Time(left.seconds() / factor);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline Time operator*(const Time& left, float32 factor)
{
  return Time(left.seconds() * factor);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_TIME_H
