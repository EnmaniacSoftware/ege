#ifndef EGE_CORE_AUDIO_NULL_SOUNDNULL_H
#define EGE_CORE_AUDIO_NULL_SOUNDNULL_H

#include "EGE.h"
#include "Core/Audio/Interface/Sound.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SoundNull : public Sound
{
  public:

    SoundNull(const String& name, PDataBuffer& data);
   ~SoundNull();

  private:

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
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIO_NULL_SOUNDNULL_H