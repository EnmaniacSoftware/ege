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

EGE_NAMESPACE

@implementation AppDelegate

- (void) dealloc
{
  // clean up
  Application::DestroyInstance(egeApplication);
  
  // deinitialize memory manager
  MemoryManager::Deinit();
  
  [super dealloc];
}

- (BOOL) application: (UIApplication*) application didFinishLaunchingWithOptions: (NSDictionary*) launchOptions
{
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
  
  EGEResult result = EGE_ERROR;

  // create application instance
  egeApplication = Application::CreateInstance();
  if (NULL != egeApplication)
  {
    // construct application
    result = egeApplication->construct(commandLineParser.dictionary());
  }

  // check for error
  if (EGE_SUCCESS != result)
  {
    // error!
    return NO;
  }
  
  // setup ticking
  CADisplayLink* displayLink = [CADisplayLink displayLinkWithTarget: self selector:@selector(tick:)];
  [displayLink addToRunLoop: [NSRunLoop currentRunLoop] forMode: NSDefaultRunLoopMode];
  
  return YES;
}

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
  }
}

- (void) applicationDidEnterBackground: (UIApplication*) application
{
  // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
  // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void) applicationWillEnterForeground: (UIApplication*) application
{
  // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void) applicationDidBecomeActive: (UIApplication*) application
{
  // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.

  assert(egeApplication);
  
  // send event
  EventManager* eventManager = egeApplication->eventManager();
  if (NULL != eventManager)
  {
    eventManager->send(EGE_EVENT_ID_CORE_APP_RESUME);
  }
}

- (void) applicationWillTerminate: (UIApplication*) application
{
  // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

- (void) touchesBegan: (NSSet*) touches withEvent: (UIEvent*) event
{
  OGLView* view = ege_cast<RenderWindowOGLIOS*>(egeApplication->graphics()->renderTarget(EGE_PRIMARY_RENDER_TARGET_NAME))->view();
  
  // go thru all touches
  for (UITouch* touch in touches)
  {
    // get location within view
    CGPoint touchPoint = [touch locationInView: view];
    
    // send pointer event
    [self notifyPointerEvent: ACTION_BUTTON_UP withButton: BUTTON_LEFT atPoint: touchPoint];
    
    egeDebug() << "Touch began at" << touchPoint.x << touchPoint.y;
  }
}

- (void) touchesEnded: (NSSet*) touches withEvent: (UIEvent*) event
{
  OGLView* view = ege_cast<RenderWindowOGLIOS*>(egeApplication->graphics()->renderTarget(EGE_PRIMARY_RENDER_TARGET_NAME))->view();
  
  // go thru all touches
  for (UITouch* touch in touches)
  {
    // get location within view
    CGPoint touchPoint = [touch locationInView: view];
    
    // send pointer event
    [self notifyPointerEvent: ACTION_BUTTON_DOWN withButton: BUTTON_LEFT atPoint: touchPoint];
    
    egeDebug() << "Touch ended at" << touchPoint.x << touchPoint.y;
  }
}

- (void) touchesCancelled: (NSSet*) touches withEvent: (UIEvent*) event
{
  
}

- (void) touchesMoved: (NSSet*) touches withEvent: (UIEvent*) event
{
  OGLView* view = ege_cast<RenderWindowOGLIOS*>(egeApplication->graphics()->renderTarget(EGE_PRIMARY_RENDER_TARGET_NAME))->view();

  // go thru all touches
  for (UITouch* touch in touches)
  {
    // get location within view
    CGPoint touchPoint = [touch locationInView: view];
     
    // send pointer event
    [self notifyPointerEvent: ACTION_MOVE withButton: BUTTON_LEFT atPoint: touchPoint];
     
    egeDebug() << "Touch moved at" << touchPoint.x << touchPoint.y;
  }
}

/*! Sends internal pointer notification. */
- (void) notifyPointerEvent: (Action) action withButton: (Button) button atPoint: (CGPoint) point
{
  EventManager* eventManager = egeApplication->eventManager();
  
  if (NULL != eventManager)
  {
    // detrmine current keyboard modifiers
    KeyboardModifiers keyboardModifiers = KM_NONE;
  
    // send pointer event
    eventManager->send(EGE_EVENT_ID_INTERNAL_POINTER_DATA,
                       ege_new PointerData(action, button, keyboardModifiers, (s32) point.x, (s32) point.y, 0));
  }
}

/*! Method called due to timer ticks. */
- (void) tick: (CADisplayLink*) displayLink
{
  // NOTE: this call returns
  egeApplication->run();
}

@end
