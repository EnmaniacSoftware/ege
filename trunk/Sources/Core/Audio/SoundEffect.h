#ifndef EGE_CORE_SOUNDEFFECT_H
#define EGE_CORE_SOUNDEFFECT_H

/** Class representing sound effect interface.
*/

#include <EGE.h>
#include <EGESignal.h>
#include <EGETime.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Sound;

EGE_DECLARE_SMART_CLASS(SoundEffect, PSoundEffect)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class SoundEffect : public Object
{
  public:

    SoundEffect() : Object(NULL) {}
    virtual ~SoundEffect() {}

  signals:

    /*! Signal emitted when effect came to completion. */
    Signal1<PSoundEffect> finished;

  public:

    /* Returns TRUE if object is valid. */
    virtual bool isValid() const = 0;
    /* Updates object. 
     * @param time  Time frame for which update should be done.
     * @param sound Sound object for which effect is being run.
     * @return  Returns TRUE if effect came to completion. Otherwise, returns FALSE.
     */
    virtual bool update(const Time& time, Sound* sound) = 0;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SOUNDEFFECT_H