#ifndef EGE_CORE_AUDIO_AUDIOMANAGERBASE_H
#define EGE_CORE_AUDIO_AUDIOMANAGERBASE_H

/** This class represents private interface for AudioManager.
 *  TODO: Rename this class so name is more descriptive and refers to playback capabilites offered
 */

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Sound, PSound)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IAudioManagerBase
{
  public:
    
    IAudioManagerBase() {}
    virtual ~IAudioManagerBase() {}

  public:

    /*! Request playback of the given sound. 
     *  @param sound        Sound to play.
     *  @return  Returns EGE_SUCCESS if sound is sucessfully started or EGE_ERROR if sound could not be started.
     *  @note This method is called from main application thread.
     */
    virtual EGEResult requestPlay(PSound sound) = 0;
    /*! Requests playback stop of the given sound. 
     *  @param  sound Sound to stop playing.
     *  @note This method is called from main application thread.
     */
    virtual void requestStop(PSound sound) = 0;
    /*! Requests playback pause of the given sound. 
     *  @param  sound Sound to pause playing.
     *  @note This method is called from main application thread.
     */
    virtual void requestPause(PSound sound) = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIO_AUDIOMANAGERBASE_H