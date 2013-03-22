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
#include "EGEDictionary.h"
#include "EGEStringList.h"
#include "EGECommandLine.h"
#include "EGEGraphics.h"

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
    if (EGE_SUCCESS == result)
    {
      // run application
      result = egeApplication->run();
    }
  }
    
  return YES;
}

- (void) applicationWillResignActive: (UIApplication*) application
{
  // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
  // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
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
}

- (void) applicationWillTerminate: (UIApplication*) application
{
  // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

@end
