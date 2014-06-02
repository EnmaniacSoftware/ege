#ifndef EGE_CORE_AUDIO_AUDIOHELPER_H
#define EGE_CORE_AUDIO_AUDIOHELPER_H

/** Audio helper class.
 */

#include "EGE.h"
#include "EGEString.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Application;
EGE_DECLARE_SMART_CLASS(Sound, PSound)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AudioHelper
{
  public:

    /*! Creates sound object.
     *  @param  application Pointer to application.
     *  @param  name        Sound name to create.
     *  @param  groupName   Name of the resource group sound data is present in.
     *  @return Created object. If error occurs, returns NULL.
     */
    static PSound CreateSound(Application* application, const String& name, const String& groupName = "");
    /*! Creates sound object and starts its playback.
     *  @param  application Pointer to application.
     *  @param  name        Sound name to create and play.
     *  @param  repeatCount Number of time sound should be repeated.
     *  @param  groupName   Name of the resource group sound data is present in.
     *  @return Created object. If error occurs, returns NULL.
     */
    static PSound CreateSoundAndPlay(Application* application, const String& name, s32 repeatCount = 0, const String& groupName = "");
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIO_AUDIOHELPER_H