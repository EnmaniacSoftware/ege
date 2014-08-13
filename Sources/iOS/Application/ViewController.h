//
//  ViewController.h
//  EGE Framework
//
//  Created by enmaniac on 4/18/13.
//  Copyright (c) 2013 Little Bee Studios. All rights reserved.
//

#include "EGEEngine.h"
#import <UIKit/UIKit.h>

@interface ViewController : UIViewController
{
  /*! Pointer to engine object. */
  EGE::Engine* engine;
}

/*! Initializes object. */
- (id) initWithApplication: (EGE::Engine*) engine;
/*! Converts native UI orientation into framework equivalent. */
- (int) convertUIOrientation: (UIInterfaceOrientation) orientation;
/*! Returns whether status bar should be hidden or not. */
- (BOOL) prefersStatusBarHidden;

@end
