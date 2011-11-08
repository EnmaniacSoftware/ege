#ifndef EGE_AIRPLAY_SOUND_PRIVATE_H
#define EGE_AIRPLAY_SOUND_PRIVATE_H

/** Sound class used with non-software mixing. 
 */

#ifndef EGE_AIRPLAY_AUDIO_SOFTWARE

#include <EGE.h>
#include <EGEDataBuffer.h>
#include <EGETime.h>
#include <EGEList.h>
#include <EGEDynamicArray.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Sound;
class AudioManagerPrivate;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class SoundPrivate
{
  public:

    SoundPrivate(Sound* base);
   ~SoundPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(Sound)

  public:

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Sets channel sound is being played on. */
    void setChannel(s32 channelId);
    /*! Returns channel id sound is being played. */
    inline s32 channel() const { return m_channelId; }

  private:

    /*! Channel sound is being played on. Negative if not being played. */
    s32 m_channelId;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // !EGE_AIRPLAY_AUDIO_SOFTWARE

#endif // EGE_AIRPLAY_SOUND_PRIVATE_H
