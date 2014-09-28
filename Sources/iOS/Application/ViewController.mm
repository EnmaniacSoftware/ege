//
//  ViewController.m
//  EGE Framework
//
//  Created by enmaniac on 4/18/13.
//  Copyright (c) 2013 Little Bee Studios. All rights reserved.
//

#import "ViewController.h"
#include "EGEEvent.h"
#include "EGEDevice.h"

EGE_NAMESPACE

@implementation ViewController

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//- (BOOL) shouldAutorotateToInterfaceOrientation: (UIInterfaceOrientation) interfaceOrientation
//{
//  return UIInterfaceOrientationIsLandscape(interfaceOrientation);
//}
////--------------------------------------------------------------------------------------------------------------------------------------------------------------
//- (NSUInteger) supportedInterfaceOrientations
//{
//  return UIInterfaceOrientationMaskLandscape;
//}
////--------------------------------------------------------------------------------------------------------------------------------------------------------------
//- (BOOL) shouldAutorotate
//{
//  return YES;
//}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (id) initWithApplication: (Engine*) egeEngine
{
  self = [super initWithNibName: nil bundle: nil];
  if (self)
  {
    // store pointer
    engine = egeEngine;
  }
  
  return self;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (int) convertUIOrientation: (UIInterfaceOrientation) orientation
{
  // map orientation to EGE compilant value
  s32 egeOrientation;
  switch (orientation)
  {
    case UIInterfaceOrientationPortrait:           egeOrientation = EOrientationPortrait; break;
    case UIInterfaceOrientationPortraitUpsideDown: egeOrientation = EOrientationPortraitUpsideDown; break;
    case UIInterfaceOrientationLandscapeLeft:      egeOrientation = EOrientationLandscapeLeft; break;
    case UIInterfaceOrientationLandscapeRight:     egeOrientation = EOrientationLandscapeRight; break;

    default:

      egeOrientation = EOrientationUnknown;
      break;
  }

  return egeOrientation;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (void) didRotateFromInterfaceOrientation: (UIInterfaceOrientation) fromInterfaceOrientation
{
  // get current UI orientation
  UIInterfaceOrientation nativeOrientation = [[UIApplication sharedApplication] statusBarOrientation];

  assert(engine);

  // send event
  IEventManager* eventManager = engine->eventManager();
  if (NULL != eventManager)
  {
    // map orientation to EGE compilant value
    s32 orientation = [self convertUIOrientation: nativeOrientation];

    // propagate event
    eventManager->send(EGE_EVENT_ID_INTERNAL_ORIENTATION_CHANGED, orientation);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (BOOL) prefersStatusBarHidden
{
  return YES;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

@end