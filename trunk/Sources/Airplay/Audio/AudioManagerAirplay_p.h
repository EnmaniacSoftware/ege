#ifndef EGE_AIRPLAY_AUDIOMANAGER_PRIVATE_H
#define EGE_AIRPLAY_AUDIOMANAGER_PRIVATE_H

#include <EGE.h>
#include <EGETime.h>
#include <EGEList.h>

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

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(AudioManager)

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Updates manager. */
    void update(const Time& time);
    /* Plays given sound. */
    EGEResult play(const PSound& sound);

  private:

    /* Airplay callback function called once sample has ended. */
    static int32 SampleEndCallback(void* systemData, void* userData);
    /* Airplay callback function called everytime when more audio data is required. */
    static int32 AudioCallback(void* systemData, void* userData);

  private:

    typedef Map<s32, PSound> ChannelsMap;

  private:

    /*! Map of sounds being currently played sorted by channels. */
    ChannelsMap m_activeSounds;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_AIRPLAY_AUDIOMANAGER_PRIVATE_H