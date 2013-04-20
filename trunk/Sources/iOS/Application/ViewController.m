//
//  ViewController.m
//  EGE Framework
//
//  Created by enmaniac on 4/18/13.
//  Copyright (c) 2013 Little Bee Studios. All rights reserved.
//

#import "ViewController.h"
#import "iOS/Graphics/OpenGL/ViewOGL.h"

@implementation ViewController

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (void) loadView
{
  // get screen size
  CGRect screenBounds = [[UIScreen mainScreen] bounds];
  
  OGLView* viewOGL = [[OGLView alloc] initWithFrame: screenBounds];

  self.view = viewOGL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (BOOL) shouldAutorotateToInterfaceOrientation: (UIInterfaceOrientation) interfaceOrientation
{
  return (UIInterfaceOrientationLandscapeLeft == interfaceOrientation) || (UIInterfaceOrientationLandscapeRight == interfaceOrientation);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

@end
