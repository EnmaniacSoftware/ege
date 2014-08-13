//
//  AppDelegate.h
//  EGE Framework
//
//  Created by enmaniac on 3/18/13.
//  Copyright (c) 2013 Little Bee Studios. All rights reserved.
//

#import <UIKit/UIKit.h>
#include "EGEEngine.h"

@interface AppDelegate : UIResponder <UIApplicationDelegate>
{
  /*! Pointer to engine object. */
  EGE::Engine* engine;
  /*! Display scale factor. */
  EGE::float32 scaleFactor;
}

@end
