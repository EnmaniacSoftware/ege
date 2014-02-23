#ifndef EGE_CORE_TIME_H
#define EGE_CORE_TIME_H

/** 
  This class encapsulates time duration.
*/

#include "EGETypes.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Time
{
  public:

    Time();
    Time(s64 microseconds);
    Time(const Time& time);
    Time(float32 seconds);

  public:
  
    /*! Returns Time object from given seconds. */
    static Time FromSeconds(float32 seconds);

  public:
  
    bool operator > (const Time& time) const;
    bool operator >= (const Time& time) const;
    bool operator < (const Time& time) const;
    bool operator <= (const Time& time) const;
    bool operator == (const Time& time);
    bool operator != (const Time& time);
  
    Time& operator = (float32 seconds);
    Time& operator = (const Time& time);
    Time& operator = (s64 microseconds);
    Time& operator += (const Time& time);
    Time& operator -= (const Time& time);

    /*! Sets time from given microseconds. */
    void fromMicroseconds(s64 microseconds);
    /*! Sets time from given miliseconds. */
    void fromMiliseconds(s64 miliseconds);
    /*! Sets time from given seconds. */
    void fromSeconds(float32 seconds);

    /*! Returns time duration in microseconds. */
    s64 microseconds() const;
    /*! Returns time duration in miliseconds. */
    s64 miliseconds() const;
    /*! Returns time duration in seconds (with fractions). */
    float32 seconds() const;
    /*! Returns time duration in minutes (with fractions). */
    float32 minutes() const;

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
inline Debug operator << (Debug debug, const Time& obj)
{
  debug.nospace() << "Time(" << obj.seconds() << " secs)";
  return debug.space();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_TIME_H
