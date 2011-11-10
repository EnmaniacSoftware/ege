#ifndef EGE_CORE_AUDIOMANAGER_OPENAL_H
#define EGE_CORE_AUDIOMANAGER_OPENAL_H

#ifdef EGE_AUDIO_OPENAL

#include <EGE.h>
#include <EGETime.h>
#include <EGEDynamicArray.h>
#include <EGEMap.h>
#include <al.h>
#include <alc.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define CHANNELS_COUNT 24
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class AudioManager;

EGE_DECLARE_SMART_CLASS(Sound, PSound)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class AudioManagerPrivate
{
  public:

    AudioManagerPrivate(AudioManager* base);
   ~AudioManagerPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(AudioManager)

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Updates manager. */
    void update(const Time& time);
    /* Plays given sound.
     * @param sound Sound to play.
     * @return  Returns EGE_SUCCESS if sound is sucessfully started or EGE_ERROR if sound could not be started.
     */
    EGEResult play(const PSound& sound);
    /* Returns TRUE if given sound is being played. */
    bool isPlaying(const PSound& sound) const;
    /* Stops playback of the given sound. */
    EGEResult stop(const PSound& sound);

  private:

    /* Returns first available channel. */
    ALuint availableChannel() const;

  private:

    /*! Audio device. */
    ALCdevice* m_device;
    /*! Audio context. */
    ALCcontext* m_context;
    /*! Available channels. */
    ALuint m_channels[CHANNELS_COUNT];
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_AUDIO_OPENAL

#endif // EGE_CORE_AUDIOMANAGER_OPENAL_H