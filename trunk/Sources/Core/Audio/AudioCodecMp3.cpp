#include "Core/Audio/AudioCodecMp3.h"
#include "EGEAudio.h"
#include "EGEMath.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(AudioCodecMp3)
EGE_DEFINE_DELETE_OPERATORS(AudioCodecMp3)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioCodecMp3::AudioCodecMp3(const PDataBuffer& stream) : AudioCodec(EGE_OBJECT_UID_AUDIO_CODEC_MP3, stream)
{
  reset();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioCodecMp3::~AudioCodecMp3()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AudioCodecMp3::decode(const PDataBuffer& out, s32 samplesCount, s32& samplesDecoded)
{
  EGE_ASSERT_X(false, "Not implemened!");

  // TAGE - for the time being this is only a stub 
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AudioCodecMp3::reset()
{
  EGE_ASSERT_X(false, "Not implemened!");

  // TAGE - for the time being this is only a stub 
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END