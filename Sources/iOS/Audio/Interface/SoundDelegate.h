//
//  SoundDelegate.h
//  EGE Framework
//
//  Created by enmaniac on 13/06/13.
//  Copyright (c) 2013 Little Bee Studios. All rights reserved.
//

#include "EGE.h"
#include "iOS/Audio/Implementation/OpenAL/SoundOpenALIOS.h"
#import <AVFoundation/AVFoundation.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
@interface SoundDelegate : NSObject <AVAudioPlayerDelegate>
{
  /*! Pointer to associated sound object .*/
  EGE::SoundOpenALIOS* m_sound;
}

/*! Initializes object with sound.
 *  @param  sound Sound with which initialize object.
 *  @return Created object.
 */
- (id) initWithSound: (EGE::SoundOpenALIOS*) sound;

/*! Callback called when playback is successfully finished. 
 *  @param  player  Audio player that finished playing.
 *  @param  flag    YES on successful completion of playback. NO if playback stopped because the system could not decode the audio data. 
 */
- (void) audioPlayerDidFinishPlaying: (AVAudioPlayer*) player successfully: (BOOL) flag;

/*! Callback called when an error occurs while decoding. 
 *  @param  player  Audio player for which event is being reported.
 *  @param  error   Reported error.
 */
- (void) audioPlayerDecodeErrorDidOccur: (AVAudioPlayer*) player error: (NSError*) error;
@end
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
