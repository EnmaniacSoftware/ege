#include "Core/Audio/AudioCodecMp3.h"
#include <EGEAudio.h>
#include <EGEMath.h>
#include <EGEDebug.h>

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
/* AudioCodec override. Decodes up to given number of samples. 
 * @param  out             Data buffer containing decoded samples.
 * @param  samplesCount    Number of samples to be decoded.
 * @param  samplesDecoded  Number of actual samples decoded. May be different than requested.
 * @return Returns TRUE if end of data has been reached. Otherwise, false.
 * @note Number of actually decoded samples may be less than requested if end of data is reached. Otherwise it should match the requested value.
 */
bool AudioCodecMp3::decode(const PDataBuffer& out, s32 samplesCount, s32& samplesDecoded)
{
  EGE_ASSERT("Not implemened!");

  // TAGE - for the time being this is only a stub 
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Resets codec. */
bool AudioCodecMp3::reset()
{
  EGE_ASSERT("Not implemened!");

  // TAGE - for the time being this is only a stub 
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END