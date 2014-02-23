#ifndef EGE_CORE_AUDIO_AUDIOMANAGERBASE_H
#define EGE_CORE_AUDIO_AUDIOMANAGERBASE_H

/** This class represents private interface for AudioManager.
 */

#include "EGE.h"
#include "Core/Event/EventListener.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Sound, PSound)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IAudioManagerBase : public IEventListener 
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

    ///*! Returns current state. */
    ////State state() const;
    ///*! Returns TRUE if given sound is being played. */
    //bool isPlaying(const PSound& sound) const;
    ///*! Returns list of sounds being played with the given name. */
    //List<PSound> sounds(const String& soundName) const;
    ///*! Pauses sound with given name. 
    // *  @param soundName   Name of the sound to pause.
    // *  @return  Returns EGE_SUCCESS if sound is sucessfully paused, EGE_ERROR_NOT_FOUND if sound could not be found or EGE_ERROR if sound could not be paused.
    // */
    //EGEResult pause(const String& soundName);
    ///*! Pauses given sound.
    // *  @param sound Sound to pause.
    // *  @return  Returns EGE_SUCCESS if sound is sucessfully paused or EGE_ERROR if sound could not be paused.
    // */
    //EGEResult pause(const PSound& sound);
    ///*! Returns TRUE if sound of a given name is paused. */
    //bool isPaused(const String& soundName) const;
    ///*! Returns TRUE if given sound is paused. */
    //bool isPaused(const PSound& sound) const;

  private slots:

    /*! Slot called when given sound stopped playback. */
    //void onStopped(PSound sound);

  private:

    /*! @ see IEventListener::onEventRecieved. */
    //void onEventRecieved(PEvent event) override;
    /*! Shuts down. */
    //void shutDown();
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIO_AUDIOMANAGERBASE_H