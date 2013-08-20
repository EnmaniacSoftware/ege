//
//  AppDelegate.mm
//  EGE Framework
//
//  Created by enmaniac on 3/18/13.
//  Copyright (c) 2013 Little Bee Studios. All rights reserved.
//

#import <Foundation/NSProcessInfo.h>
#import <Foundation/NSString.h>
#import <Foundation/NSArray.h>
#import "iOS/Application/AppDelegate.h"
#import "iOS/Graphics/OpenGL/RenderWindowOGLIOS.h"
#import "iOS/Graphics/OpenGL/ViewOGL.h"
#include "EGEDictionary.h"
#include "EGEStringList.h"
#include "EGECommandLine.h"
#include "EGEGraphics.h"
#include "EGEEvent.h"
#include "EGEInput.h"
#include "EGEDevice.h"

EGE_NAMESPACE

@implementation AppDelegate

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* KAppDelegateDebugName = "EGEAppDelegate";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (void) dealloc
{
  // clean up
  Application::DestroyInstance(egeApplication);
  
  // deinitialize memory manager
  MemoryManager::Deinitialize();
  
  [super dealloc];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (CGPoint) transformTouchPoint: (CGPoint) point
{
  // get current UI orientation
  UIInterfaceOrientation orientation = [[UIApplication sharedApplication] statusBarOrientation];

  // transform due to current UI orientation
  CGPoint newPoint = point;
    
  if (UIInterfaceOrientationLandscapeRight == orientation)
  {
    newPoint.x = point.y;
    newPoint.y = Device::SurfaceWidth() - point.x;
  }
  else if (UIInterfaceOrientationLandscapeLeft == orientation)
  {
    newPoint.x = Device::SurfaceHeight() - point.y;
    newPoint.y = point.x;
  }
  
  return newPoint;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (void) registerForDeviceOrientationChanges: (UIApplication*) application
{
  UIDevice* device = [UIDevice currentDevice];
  
  // enable device orientation events
  [device beginGeneratingDeviceOrientationNotifications];

  // register for notifications
  NSNotificationCenter* notificationCenter = [NSNotificationCenter defaultCenter];
	[notificationCenter addObserver: self selector: @selector(onDeviceOrientationChanged:) name: UIDeviceOrientationDidChangeNotification object: device];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (void) onDeviceOrientationChanged: (NSNotification*) notification
{
  // get orientation value
  int nativeOrientation = [[notification object] orientation];
  
  egeDebug(KAppDelegateDebugName) << "Device orientation has changed:" << nativeOrientation;

  assert(egeApplication);
  
  // send event
  EventManager* eventManager = egeApplication->eventManager();
  if (NULL != eventManager)
  {
    // map orientation to EGE compilant value
    s32 orientation;
    switch (nativeOrientation)
    {
      case UIDeviceOrientationPortrait:           orientation = EGEDevice::EOrientationPortrait; break;
      case UIDeviceOrientationPortraitUpsideDown: orientation = EGEDevice::EOrientationPortraitUpsideDown; break;
      case UIDeviceOrientationLandscapeLeft:      orientation = EGEDevice::EOrientationLandscapeLeft; break;
      case UIDeviceOrientationLandscapeRight:     orientation = EGEDevice::EOrientationLandscapeRight; break;
      case UIDeviceOrientationFaceUp:             orientation = EGEDevice::EOrientationFaceUp; break;
      case UIDeviceOrientationFaceDown:           orientation = EGEDevice::EOrientationFaceDown; break;
      
      default:
      
        orientation = EGEDevice::EOrientationUnknown;
        break;
    }
    
    eventManager->send(EGE_EVENT_ID_INTERNAL_ORIENTATION_CHANGED, orientation);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (BOOL) application: (UIApplication*) application didFinishLaunchingWithOptions: (NSDictionary*) launchOptions
{
  BOOL result = NO;

  // initialize memory manager
  if (MemoryManager::Initialize())
  {
    result = YES;

    // hide status bar
    application.statusBarHidden = YES;
    //application.statusBarOrientation = UIInterfaceOrientationLandscapeLeft;
  
    // register for device orientation changes
    //[self registerForDeviceOrientationChanges: application];
  
    // check current directory
    NSLog(@"Current dir: %@", [[NSFileManager defaultManager] currentDirectoryPath]);

    // force change to bundle current directory
    [[NSFileManager defaultManager] changeCurrentDirectoryPath: [[NSBundle mainBundle] bundlePath]];

    NSLog(@"Current dir (after change): %@", [[NSFileManager defaultManager] currentDirectoryPath]);
  
    // process command line 
	  NSArray* commandLineArguments = [[NSProcessInfo processInfo] arguments];

    StringList argumentList;
    for (NSString* argument in commandLineArguments)
    {
      const char* arg = [argument cStringUsingEncoding: NSASCIIStringEncoding];
      argumentList << arg;
    }
  
    // parse command line
    CommandLineParser commandLineParser(argumentList);
  
    EGEResult value = EGE_ERROR;

    // create application instance
    egeApplication = Application::CreateInstance();
    if (NULL != egeApplication)
    {
      // construct application
      value = egeApplication->construct(commandLineParser.dictionary());
    }

    // check for error
    if (EGE_SUCCESS != value)
    {
      // error!
      result = NO;
    }
  
	  // disable screen dimming
	  [UIApplication sharedApplication].idleTimerDisabled = YES;
  
    // setup ticking
    CADisplayLink* displayLink = [CADisplayLink displayLinkWithTarget: self selector:@selector(tick:)];
    [displayLink addToRunLoop: [NSRunLoop currentRunLoop] forMode: NSDefaultRunLoopMode];
    [displayLink setFrameInterval: 1];
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (void) applicationWillResignActive: (UIApplication*) application
{
  // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
  // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.

  assert(egeApplication);

  // send event
  EventManager* eventManager = egeApplication->eventManager();
  if (NULL != eventManager)
  {
    eventManager->send(EGE_EVENT_ID_CORE_APP_PAUSE);
    
    // disable rendering
    egeApplication->graphics()->setRenderingEnabled(false);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (void) applicationDidEnterBackground: (UIApplication*) application
{
  // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
  // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (void) applicationWillEnterForeground: (UIApplication*) application
{
  // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (void) applicationDidBecomeActive: (UIApplication*) application
{
  // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.

  assert(egeApplication);
  
  // send event
  EventManager* eventManager = egeApplication->eventManager();
  if (NULL != eventManager)
  {
    eventManager->send(EGE_EVENT_ID_CORE_APP_RESUME);

    // enable rendering
    egeApplication->graphics()->setRenderingEnabled(true);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (void) applicationWillTerminate: (UIApplication*) application
{
  // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
 
  // disable orientation events
  [[UIDevice currentDevice] endGeneratingDeviceOrientationNotifications];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (void) touchesBegan: (NSSet*) touches withEvent: (UIEvent*) event
{
  // go thru all touches
  for (UITouch* touch in touches)
  {
    // get location within view
    CGPoint point = [touch locationInView: nil];
    point = [self transformTouchPoint: point];
    
    // send pointer event
    [self notifyPointerEvent: ACTION_BUTTON_DOWN withButton: BUTTON_LEFT atPoint: point];
    
    egeDebug(KAppDelegateDebugName) << "Touch began at" << point.x << point.y << (float32) touch.timestamp;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (void) touchesEnded: (NSSet*) touches withEvent: (UIEvent*) event
{
  // go thru all touches
  for (UITouch* touch in touches)
  {
    // get location within view
    CGPoint point = [touch locationInView: nil];
    point = [self transformTouchPoint: point];
    
    // send pointer event
    [self notifyPointerEvent: ACTION_BUTTON_UP withButton: BUTTON_LEFT atPoint: point];
    
    egeDebug(KAppDelegateDebugName) << "Touch ended at" << point.x << point.y << (float32) touch.timestamp;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (void) touchesCancelled: (NSSet*) touches withEvent: (UIEvent*) event
{
  // process as touch end
  [self touchesEnded: touches withEvent: event];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (void) touchesMoved: (NSSet*) touches withEvent: (UIEvent*) event
{
  // go thru all touches
  for (UITouch* touch in touches)
  {
    // get location within view
    CGPoint point = [touch locationInView: nil];
    point = [self transformTouchPoint: point];
    
    // send pointer event
    [self notifyPointerEvent: ACTION_MOVE withButton: BUTTON_LEFT atPoint: point];
     
    egeDebug(KAppDelegateDebugName) << "Touch moved at" << point.x << point.y << (float32) touch.timestamp;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sends internal pointer notification. */
- (void) notifyPointerEvent: (Action) action withButton: (Button) button atPoint: (CGPoint) point
{
  EventManager* eventManager = egeApplication->eventManager();
  
  if (NULL != eventManager)
  {
    // detrmine current keyboard modifiers
    KeyboardModifiers keyboardModifiers = KM_NONE;
    
    // send pointer event
    PObject data = ege_new PointerData(action, button, keyboardModifiers, (s32) point.x, (s32) point.y, 0);
    eventManager->send(EGE_EVENT_ID_INTERNAL_POINTER_DATA, data);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Method called due to timer ticks. */
- (void) tick: (CADisplayLink*) displayLink
{
  // NOTE: this call returns
  egeApplication->run();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

@end
