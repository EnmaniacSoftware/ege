#include "Core/Audio/Sound.h"
#include "Core/Audio/AudioCodecWav.h"
#include "Core/Audio/AudioCodecOgg.h"
#include "Core/Audio/AudioCodecMp3.h"
#include "Core/Audio/AudioUtils.h"
#include "EGEApplication.h"
#include "EGEResources.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KSoundDebugName = "EGESound";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Sound)
EGE_DEFINE_DELETE_OPERATORS(Sound)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Sound::Sound(Application* app, const String& name, PDataBuffer& data) : Object(app),
                                                                        m_codec(NULL),
                                                                        m_name(name),
                                                                        m_data(data)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Sound::~Sound()
{
  EGE_DELETE(m_codec);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PSound Sound::CreateAndPlay(Application* application, const String& name, s32 repeatCount, const String& resourceGroupName)
{
  EGE_ASSERT(NULL != application);

  PSound sound;

  // find sound resource
  PResourceSound soundResource = application->resourceManager()->soundResource(name, resourceGroupName);
  if (NULL != soundResource)
  {
    // create sound instance
    sound = soundResource->createInstance();
    if (NULL != sound)
    {
      // play
      if (EGE_SUCCESS != sound->play(repeatCount))
      {
        // error!
        sound = NULL;
      }
    }
  }

  return sound;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult Sound::construct()
{
  // detect stream type
  Audio::StreamType type = AudioUtils::DetectStreamType(m_data);

  switch (type)
  {
    case Audio::ST_WAVE:
        
      m_codec = ege_new AudioCodecWav(m_data);
      break;

    case Audio::ST_OGG:

      m_codec = ege_new AudioCodecOgg(m_data);
      break;
      
    case Audio::ST_MP3:

      m_codec = ege_new AudioCodecMp3(m_data);
      break;

    default:

      EGE_ASSERT_X(false, "Unsupported audio format!");
      return EGE_ERROR_NOT_SUPPORTED;
  }

  if (NULL == m_codec)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  return EGE_SUCCESS;
}
////--------------------------------------------------------------------------------------------------------------------------------------------------------------
//void Sound::update(const Time& time)
//{
//  EGE_UNUSED(time);
//}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& Sound::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Sound::addEffect(PSoundEffect effect)
{
  if (NULL != effect)
  {
    m_effects.push_back(effect);

    // do initial update
    effect->update(0LL, this);
    return true;
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundEffectList Sound::effects(u32 uid) const
{
  SoundEffectList list;

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::notifyFinished()
{
  emit finished(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::notifyStopped()
{
  emit stopped(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::notifyVolumeChanged(float32 oldVolume)
{
  egeDebug(KSoundDebugName) << "Volume change [" << name() << "] " << oldVolume << "to" << volume();

  emit volumeChanged(this, oldVolume);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::updateSoundEffects(const Time& time)
{
  for (SoundEffectList::iterator it = m_effects.begin(); it != m_effects.end(); )
  {
    PSoundEffect& effect = *it;

    // update
    if (effect->update(time, this))
    {
      // effect is over, remove
      it = m_effects.erase(it);
    }
    else
    {
      ++it;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::update(const Time& time)
{
  updateSoundEffects(time);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

