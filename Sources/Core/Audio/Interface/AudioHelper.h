#ifndef EGE_CORE_AUDIO_AUDIOHELPER_H
#define EGE_CORE_AUDIO_AUDIOHELPER_H

/** Audio helper class.
 */

#include "EGE.h"
#include "EGEString.h"
#include "EGETime.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Engine;
EGE_DECLARE_SMART_CLASS(Sound, PSound)
EGE_DECLARE_SMART_CLASS(SoundEffect, PSoundEffect)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AudioHelper
{
  public:

    /*! Creates sound object.
     *  @param  engine      Reference to engine.
     *  @param  name        Sound name to create.
     *  @param  groupName   Name of the resource group sound data is present in.
     *  @return Created object. If error occurs, returns NULL.
     */
    static PSound CreateSound(Engine& engine, const String& name, const String& groupName = "");
    /*! Creates sound object and starts its playback.
     *  @param  engine      Reference to engine.
     *  @param  name        Sound name to create and play.
     *  @param  repeatCount Number of time sound should be repeated.
     *  @param  groupName   Name of the resource group sound data is present in.
     *  @return Created object. If error occurs, returns NULL.
     */
    static PSound CreateSoundAndPlay(Engine& engine, const String& name, s32 repeatCount = 0, const String& groupName = "");

    /*! Fades in the given sound.
     *  @param  sound     Sound to fade in.
     *  @param  duration  Fade in duration.
     *  @return Fade in effect on success. If error occurs, returns NULL.
     */
    static PSoundEffect FadeIn(PSound& sound, const Time& duration);
    /*! Fades out the given sound.
     *  @param  sound     Sound to fade out.
     *  @param  duration  Fade in duration.
     *  @return Fade out effect on success. If error occurs, returns NULL.
     */
    static PSoundEffect FadeOut(PSound& sound, const Time& duration);
    /*! Fades out the given sound and stops it when effect is done.
     *  @param  sound     Sound to fade out.
     *  @param  duration  Fade in duration.
     *  @return Fade out effect on success. If error occurs, returns NULL.
     */
    static PSoundEffect FadeOutAndStop(PSound& sound, const Time& duration);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIO_AUDIOHELPER_H