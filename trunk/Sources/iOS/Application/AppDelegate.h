//
//  AppDelegate.h
//  EGE Framework
//
//  Created by enmaniac on 3/18/13.
//  Copyright (c) 2013 Little Bee Studios. All rights reserved.
//

#import <UIKit/UIKit.h>
#include "Core/Application/Application.h"

@interface AppDelegate : UIResponder <UIApplicationDelegate>
{
  /*! Application object. */
  EGE::Application* egeApplication;
}

@end
