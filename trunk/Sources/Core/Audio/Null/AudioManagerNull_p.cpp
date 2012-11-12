#ifdef EGE_AUDIO_NULL

#include "Core/Application/Application.h"
#include "Core/Audio/Null/AudioManagerNull_p.h"
#include "Core/Audio/AudioManager.h"
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(AudioManagerPrivate)
EGE_DEFINE_DELETE_OPERATORS(AudioManagerPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerPrivate::AudioManagerPrivate(AudioManager* base) : m_d(base)
{
  EGE_UNUSED(base)
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioManagerPrivate::~AudioManagerPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Constructs object. */
EGEResult AudioManagerPrivate::construct()
{
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates manager. */
void AudioManagerPrivate::update(const Time& time)
{
  EGE_UNUSED(time)
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Plays given sound.
 * @param sound Sound to play.
 * @return  Returns EGE_SUCCESS if sound is sucessfully started or EGE_ERROR if sound could not be started.
 */
EGEResult AudioManagerPrivate::play(const PSound& sound)
{
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Stops playback of the given sound. */
EGEResult AudioManagerPrivate::stop(const PSound& sound)
{
  EGE_UNUSED(sound)

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if given sound is being played. */
bool AudioManagerPrivate::isPlaying(const PSound& sound) const
{
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Pauses given sound.
 *  @param sound Sound to pause.
 *  @return  Returns EGE_SUCCESS if sound is sucessfully paused or EGE_ERROR if sound could not be paused.
 */
EGEResult AudioManagerPrivate::pause(const PSound& sound)
{
  EGE_UNUSED(sound)

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if given sound is paused. */
bool AudioManagerPrivate::isPaused(const PSound& sound) const
{
  EGE_UNUSED(sound)

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if given sound is stopped. */
bool AudioManagerPrivate::isStopped(const PSound& sound) const
{
  EGE_UNUSED(sound)

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Shuts manager down. */
void AudioManagerPrivate::shutDown() 
{
  d_func()->m_state = AudioManager::STATE_CLOSED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_AUDIO_NULL