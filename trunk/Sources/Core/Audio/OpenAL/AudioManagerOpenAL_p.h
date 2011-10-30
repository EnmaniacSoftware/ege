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

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Updates manager. */
    void update(const Time& time);
    /* Plays given sound. */
    EGEResult play(const PSound& sound);

  private:

    /* Returns first available channel. */
    ALuint availableChannel() const;

  private:

    typedef DynamicArray<ALuint> ChannelsArray;
    typedef Map<ALuint, PSound> ChannelsMap;

  private:

    /*! Audio device. */
    ALCdevice* m_device;
    /*! Audio context. */
    ALCcontext* m_context;
    /*! Available channels. */
    ChannelsArray m_channels;
    /*! Map of sounds being currently played sorted by channels. */
    ChannelsMap m_activeSounds;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_AUDIO_OPENAL

#endif // EGE_CORE_AUDIOMANAGER_OPENAL_H