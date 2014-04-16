#ifndef EGE_CORE_TIME_TIME_H
#define EGE_CORE_TIME_TIME_H

/*! This class encapsulates time duration.
 */

#include "EGE.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Time
{
  public:

    Time();
    Time(const Time& time);
    Time(s64 microseconds);
    Time(float32 seconds);

  operators:
  
    bool operator > (const Time& other) const;
    bool operator >= (const Time& other) const;
    bool operator < (const Time& other) const;
    bool operator <= (const Time& other) const;
    bool operator == (const Time& other) const;
    bool operator != (const Time& other) const;
  
    Time& operator = (const Time& other);
    Time& operator = (float32 seconds);
    Time& operator = (s64 microseconds);
    
    Time& operator += (const Time& other);
    Time& operator -= (const Time& other);

  public:

    /*! Sets time from given miliseconds. */
    void fromMiliseconds(s64 miliseconds);

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
inline Time operator + (const Time& left, const Time& right)
{
  return Time(left.microseconds() + right.microseconds());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline Time operator - (const Time& left, const Time& right)
{
  return Time(left.microseconds() - right.microseconds());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline Time operator / (const Time& left, float32 factor)
{
  return Time(left.seconds() / factor);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline Time operator * (const Time& left, float32 factor)
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

#endif // EGE_CORE_TIME_TIME_H
