#import "SoundDelegate.h"

EGE_NAMESPACE

@implementation SoundDelegate

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (id) initWithSound: (SoundOpenALIOS*) sound
{
  self = [super init];
  if (self)
  {
    self->m_sound = sound;
  }
  
  return self;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (void) audioPlayerDidFinishPlaying: (AVAudioPlayer*) player successfully: (BOOL) flag
{
  m_sound->setState(SoundOpenAL::StateStopped);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (void) audioPlayerDecodeErrorDidOccur: (AVAudioPlayer*) player error: (NSError*) error
{
  m_sound->setState(SoundOpenAL::StateStopped);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

@end
