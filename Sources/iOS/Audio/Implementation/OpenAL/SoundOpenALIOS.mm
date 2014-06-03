#include "iOS/Audio/Implementation/OpenAL/SoundOpenALIOS.h"
#include "iOS/Audio/Interface/OpenAL/AudioManagerOpenALIOS.h"
#include "Core/Audio/Implementation/Codecs/AudioCodec.h"
#import "iOS/Audio/Interface/SoundDelegate.h"
#import <AVFoundation/AVFoundation.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(SoundOpenALIOS)
EGE_DEFINE_DELETE_OPERATORS(SoundOpenALIOS)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundOpenALIOS::SoundOpenALIOS(AudioManagerOpenALIOS* manager, const String& name, PDataBuffer& data) : SoundOpenAL(manager, name, data),
                                                                                                        m_audioPlayer(nil),
                                                                                                        m_delegate(nil)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundOpenALIOS::~SoundOpenALIOS()
{
  [m_delegate release];
  m_delegate = nil;
  
  [m_audioPlayer release];
  m_audioPlayer = nil;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SoundOpenALIOS::construct()
{
  // call base class
  EGEResult result = SoundOpenAL::construct();
  
  // check if MP3 stream
  if ((EGE_ERROR_NOT_SUPPORTED == result) && (EGE_OBJECT_UID_AUDIO_CODEC_MP3 == m_codec->uid()))
  {
    result = EGE_SUCCESS;
    
    // allocate delegate
    m_delegate = [[SoundDelegate alloc] initWithSound: this];
    
    if (nil == m_delegate)
    {
      // error!
      result = EGE_ERROR_NO_MEMORY;
    }
  }
  
  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SoundOpenALIOS::update(const Time& time)
{
  // check if MP3 (or other supported compressed audio)
  if (EGE_OBJECT_UID_AUDIO_CODEC_MP3 == m_codec->uid())
  {
    Sound::update(time);
  }
  else
  {
    SoundOpenAL::update(time);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SoundOpenALIOS::doPlay(ALuint channel)
{
  bool result = false;
  
  // check if MP3 (or other supported compressed audio)
  if (EGE_OBJECT_UID_AUDIO_CODEC_MP3 == m_codec->uid())
  {
    const PObject& stream = m_codec->stream();

    // process according to stream type
    switch (stream->uid())
    {
      case EGE_OBJECT_UID_DATA_BUFFER:
      
        // check if NOT created already
        if (nil == m_audioPlayer)
        {
          NSData* data = nil;
          NSError* error = nil;

          // wrap data
          data = [[NSData alloc] initWithBytesNoCopy: ege_cast<DataBuffer*>(stream)->data() length: ege_cast<DataBuffer*>(stream)->size() freeWhenDone: NO];
        
          // create audio player
          m_audioPlayer = [[AVAudioPlayer alloc] initWithData: data error: &error];
          [data release];
          
          // check if error
          if (nil != error)
          {
            // error!
            return false;
          }
          
          // attach delegate
          m_audioPlayer.delegate = m_delegate;
        }
        else
        {
          // rewind
          [m_audioPlayer setCurrentTime: 0];
        }
        
        // prepare for playback
        [m_audioPlayer prepareToPlay];
        
        // set volume
        m_audioPlayer.volume = volume();
        
        // set number of loops
        m_audioPlayer.numberOfLoops = repeatCount();
        
        // play
        [m_audioPlayer play];
        
        // set state
        setState(StatePlaying);
        
        // set result
        result = true;
        break;
            
      default:
        
        // unsupported
        result = false;
        break;
    }
  }
  // check if WAV file
  else if (EGE_OBJECT_UID_AUDIO_CODEC_WAV == m_codec->uid())
  {
    // call base class
    result = SoundOpenAL::doPlay(channel);
  }
  
  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SoundOpenALIOS::doStop()
{
  bool result = true;

  // check if MP3 (or other supported compressed audio)
  if (EGE_OBJECT_UID_AUDIO_CODEC_MP3 == m_codec->uid())
  {
    [m_audioPlayer stop];
    
    // reset data
    setVolume(1.0f);
  
    // notify stopped
    notifyStopped();
  
    // set state
    setState(SoundOpenAL::StateStopped);
  }
  else
  {
    result = SoundOpenAL::doStop();
  }
  
  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SoundOpenALIOS::doPause()
{
  bool result = true;
  
  // check if MP3 (or other supported compressed audio)
  if (EGE_OBJECT_UID_AUDIO_CODEC_MP3 == m_codec->uid())
  {
    [m_audioPlayer pause];
    
    // set state
    setState(StatePaused);
  }
  else
  {
    result = SoundOpenAL::doPause();
  }
  
  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SoundOpenALIOS::doResume()
{
  bool result = true;
  
  // check if MP3 (or other supported compressed audio)
  if (EGE_OBJECT_UID_AUDIO_CODEC_MP3 == m_codec->uid())
  {
    [m_audioPlayer play];
    
    // set state
    setState(StatePlaying);
  }
  else
  {
    result = SoundOpenAL::doResume();
  }
  
  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SoundOpenALIOS::setVolume(float32 volume)
{
  // call base class
  SoundOpenAL::setVolume(volume);
  
  if (nil != m_audioPlayer)
  {
    m_audioPlayer.volume = volume;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SoundOpenALIOS::setState(State state)
{
  SoundOpenAL::setState(state);
}