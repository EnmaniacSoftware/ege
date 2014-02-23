//
//  ViewController.h
//  EGE Framework
//
//  Created by enmaniac on 4/18/13.
//  Copyright (c) 2013 Little Bee Studios. All rights reserved.
//

#include "EGEApplication.h"
#import <UIKit/UIKit.h>

@interface ViewController : UIViewController
{
  /*! Pointer to application object. */
  EGE::Application* egeApplication;
}

/*! Initializes object. */
- (id) initWithApplication: (EGE::Application*) application;
/*! Converts native UI orientation into framework equivalent. */
- (int) convertUIOrientation: (UIInterfaceOrientation) orientation;

@end
