//
//  ViewOGL.mm
//  EGE Framework
//
//  Created by enmaniac on 21/03/13.
//  Copyright (c) 2013 Little Bee Studios. All rights reserved.
//

#include "EGE.h"
#import "iOS/Graphics/OpenGL/ViewOGL.h"
#import <OpenGLES/EAGL.h>
#import <UIKit/UIScreen.h>

@implementation OGLView

@synthesize mEAGLLayer;

+ (Class) layerClass
{
  return [CAEAGLLayer class];
}

- (id) initWithFrame: (CGRect) frame andPixelFormat: (NSString*) format
{
  self = [super initWithFrame: frame];
  if (self)
  {
    mEAGLLayer        = (CAEAGLLayer*) self.layer;
    mEAGLLayer.opaque = YES;
    mEAGLLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                     format, kEAGLDrawablePropertyColorFormat,
                                     nil];
    
    CGFloat scale = 1.0f;
    
    // get main screen
    UIScreen* screen = [UIScreen mainScreen];
    
    // check if scle selector is present
    if ([screen respondsToSelector: @selector(scale)])
    {
      // get scale factor
      scale = screen.scale;
    }
    
    [self setContentScaleFactor: scale];
  }
  
  return self;
}

/*- (void) setupRenderBuffer
{
  glGenRenderbuffers(1, &mColorRenderBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, mColorRenderBuffer);
  [mEAGLContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:mEAGLLayer];
}*/

/*
- (void)setupFrameBuffer
{
  GLuint framebuffer;
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                            GL_RENDERBUFFER, mColorRenderBuffer);
}
*/
//- (void) setupRenderBuffer
//{
//  glGenRenderbuffers(1, &mColorRenderBuffer);
//  glBindRenderbuffer(GL_RENDERBUFFER, mColorRenderBuffer);
//  [mEAGLContext renderbufferStorage: GL_RENDERBUFFER fromDrawable: mEAGLLayer];
//}

//- (void) setupFrameBuffer
//{
//  GLuint framebuffer;
//  glGenFramebuffers(1, &framebuffer);
//  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
//  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, mColorRenderBuffer);
//}

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    // Drawing code
}
*/

@end
