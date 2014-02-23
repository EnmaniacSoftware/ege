//
//  GameCenterDelegate.h
//  EGE Framework
//
//  Created by enmaniac on 17/06/13.
//  Copyright (c) 2013 Little Bee Studios. All rights reserved.
//

#include "EGE.h"
#include "iOS/Services/SocialServicesGameCenterIOS_p.h"
#import <GameKit/GameKit.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
@interface GameCenterDelegate : NSObject <GKGameCenterControllerDelegate>
{
  /*! Pointer to associated sound object .*/
  EGE::SocialServicesPrivate* m_object;
}

/*! Initializes object.
 *  @param  object  Object with which initialize object.
 *  @return Created object.
 */
- (id) initWithObject: (EGE::SocialServicesPrivate*) object;

/*! Called when the player is done interacting with the view controller’s content. 
 *  @param  GKGameCenterViewController  The view controller the player finished interacting with.
 */
- (void) gameCenterViewControllerDidFinish: (GKGameCenterViewController*) gameCenterViewController;

@end
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
