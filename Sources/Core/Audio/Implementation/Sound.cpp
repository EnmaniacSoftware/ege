#include "Core/Audio/Interface/Sound.h"
#include "Core/Audio/Implementation/Codecs/AudioCodecWav.h"
#include "Core/Audio/Implementation/Codecs/AudioCodecOgg.h"
#include "Core/Audio/Implementation/Codecs/AudioCodecMp3.h"
#include "Core/Audio/Implementation/AudioUtils.h"
#include "EGEApplication.h"
#include "EGEResources.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KSoundDebugName = "EGESound";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Sound)
EGE_DEFINE_DELETE_OPERATORS(Sound)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Sound::Sound(Application* app, const String& name, const PDataBuffer& data) : Object(app)
                                                                            , m_codec(NULL)
                                                                            , m_name(name)
                                                                            , m_data(data)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Sound::~Sound()
{
  EGE_DELETE(m_codec);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult Sound::construct()
{
  // wrap original data within local buffer
  // NOTE: This method is usually called from outside of the Audio thread. It means that internal m_data offsets can be modified from two threads at the same
  //       time. This can lead to some undefined behavior. Since, usually after construction all processing is done Audio thread alone, extra care needs to be
  //       taken here where this is generally not the case.
  DataBuffer data(m_data->data(), m_data->size());

  // detect stream type
  AudioStreamType type = AudioUtils::DetectStreamType(data);

  // NOTE: AudioCodecs guarantee internal offsets of data buffers are not changed
  switch (type)
  {
    case AST_WAV:
        
      m_codec = ege_new AudioCodecWav(m_data);
      break;

    case AST_OGG:

      m_codec = ege_new AudioCodecOgg(m_data);
      break;
      
    case AST_MP3:

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

  // go thru all effects
  for (SoundEffectList::const_iterator it = m_effects.begin(); it != m_effects.end(); ++it)
  {
    const PSoundEffect& effect = *it;

    // check if effect should be added
    if ((effect->uid() == uid) || (EGE_OBJECT_UID_GENERIC == uid))
    {
      list.push_back(effect);
    }
  }

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::notifyFinished()
{
  emit finishedWithSelf(*this);
  emit finished();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::notifyStopped()
{
  emit stoppedWithSelf(*this);
  emit stopped();
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

