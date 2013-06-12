#ifndef EGE_AIRPLAY_AUDIO_SOUNDAIRPLAY_H
#define EGE_AIRPLAY_AUDIO_SOUNDAIRPLAY_H

#include "EGE.h"
#include "Core/Audio/Sound.h"
#include "EGEDataBuffer.h"
#include "EGETime.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AudioManagerAirplay;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SoundAirplay : public Sound
{
  public:

    SoundAirplay(AudioManagerAirplay* manager, const String& name, PDataBuffer& data);
   ~SoundAirplay();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! @see Sound::construct. */
    EGEResult construct() override;
    /*! @see Sound::update. */
    void update(const Time& time) override;
    /*! @see Sound::setPitch. */
    void setPitch(float32 value) override;
    /*! @see Sound::pitch. */
    float32 pitch() const override;
    /*! @see Sound::setVolume. */
    void setVolume(float32 volume) override;
    /*! @see Sound::volume. */
    float32 volume() const override;
    /*! @see Sound::play. */
    EGEResult play(s32 repeatCount) override;
    /*! @see Sound::isPlaying. */
    bool isPlaying() const override;
    /*! @see Sound::isPaused. */
    bool isPaused() const override;
    /*! @see Sound::stop. */
    void stop() override;
    /*! @see Sound::pause. */
    void pause() override;

    /*! Returns TRUE if sound is stopped. */
    bool isStopped() const;

  private:

    /*! Pointer to manager. */
    AudioManagerAirplay* m_manager;
    /*! Channel sound is being played on. */
    s32 m_channel;
    /*! Pitch value. Each reduction by 50% repesents 1 octave reduction. Each doubling represents 1 octave increase. */
    float32 m_pitch;
    /*! Volume in [0-1] range. */
    float32 m_volume;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_AIRPLAY_AUDIO_SOUNDAIRPLAY_H