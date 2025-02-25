//
//  AppDelegate.h
//  EGE Framework
//
//  Created by enmaniac on 3/18/13.
//  Copyright (c) 2013 Little Bee Studios. All rights reserved.
//

#import <UIKit/UIKit.h>
#include "EGEApplication.h"

@interface AppDelegate : UIResponder <UIApplicationDelegate>
{
  /*! Pointer to application object. */
  EGE::Application* egeApplication;
  /*! Display scale factor. */
  EGE::float32 scaleFactor;
}

@end
