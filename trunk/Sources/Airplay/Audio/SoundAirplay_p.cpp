#ifndef EGE_AIRPLAY_AUDIO_SOFTWARE
#include "Core/Application/Application.h"
#include "Core/Audio/wsfir.h"
#include "Airplay/Audio/SoundAirplay_p.h"
#include <s3eSound.h>
#include <s3e.h>
#include <EGEDebug.h>
#include <EGEAudio.h>
#include <EGEDevice.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(SoundPrivate)
EGE_DEFINE_DELETE_OPERATORS(SoundPrivate)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundPrivate::SoundPrivate(Sound* base) : m_d(base),
                                          m_channelId(-1)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundPrivate::~SoundPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool SoundPrivate::isValid() const
{
  return true;;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets channel sound is being played on. */
void SoundPrivate::setChannel(s32 channelId)
{
  m_channelId = channelId;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#endif // !EGE_AIRPLAY_AUDIO_SOFTWARE