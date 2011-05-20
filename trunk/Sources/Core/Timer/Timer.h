#ifndef EGE_CORE_TIMER_H
#define EGE_CORE_TIMER_H

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(Timer, PTimer)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Timer : public Object
{
  public:

    Timer(Application* app);
    virtual ~Timer();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if object is valid. */
    bool isValid() const;
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

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(Timer);
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_TIMER_H
