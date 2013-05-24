//
//  ViewOGL.h
//  EGE Framework
//
//  Created by enmaniac on 21/03/13.
//  Copyright (c) 2013 Little Bee Studios. All rights reserved.
//

#import <UIKit/UIView.h>
#import <QuartzCore/QuartzCore.h>

@interface OGLView : UIView
{
  /*! OpenGLES drawing layer. */
  CAEAGLLayer* mEAGLLayer;
}

@property (nonatomic, readonly) CAEAGLLayer* mEAGLLayer;

/*! Initializes object. 
 *  @param  frame   Frame rectangle the view should cover.
 *  @param  format  String constant describing pixel format.
 *  @return Created object.
 */
- (id) initWithFrame: (CGRect) frame andPixelFormat: (NSString*) format;

@end
