#ifndef EGE_CORE_AUDIO_SOUNDEFFECT_H
#define EGE_CORE_AUDIO_SOUNDEFFECT_H

/** Class representing sound effect interface.
*/

#include "EGE.h"
#include "EGESignal.h"
#include "EGETime.h"
#include "EGEList.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(SoundEffect, PSoundEffect)
EGE_DECLARE_SMART_CLASS(Sound, PSound)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SoundEffect : public Object
{
  public:

    virtual ~SoundEffect() {}

  public signals:

    /*! Signal emitted when effect comes to completion. 
     *  @param effect Effect which is finished.
     *  @param sound  Sound for to which effect was bound while being finished.
     */
    Signal2<PSoundEffect, PSound> finishedWithSelfAndSound;
    /*! Signal emitted when effect comes to completion. */
    Signal0<> finished;

  public:

    /*! Updates object. 
     *  @param time  Time frame for which update should be done.
     *  @param sound Sound object for which effect is being run.
     *  @return  Returns TRUE if effect came to completion. Otherwise, returns FALSE.
     */
    virtual bool update(const Time& time, PSound sound) = 0;
    /*! Resets effect. */
    virtual void reset() = 0;

  protected:

    SoundEffect(u32 uid) : Object(NULL, uid) {}
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef List<PSoundEffect> SoundEffectList;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIO_SOUNDEFFECT_H