//
//  MailComposeDelegate.m
//  EGE Framework
//
//  Created by enmaniac on 7/13/14.
//  Copyright (c) 2014 Little Bee Studios. All rights reserved.
//

#import "MailComposeDelegate.h"
#include "EGEEngine.h"
#include "EGEGraphics.h"
#include "iOS/Graphics/OpenGL/RenderWindowOGLIOS.h"

EGE_NAMESPACE

@implementation MailComposeDelegate

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (id) initWithObject: (DeviceServicesIOS*) object
{
  self = [super init];
  if (self)
  {
    self->m_object = object;
  }
  
  return self;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (void) mailComposeController: (MFMailComposeViewController*) controller didFinishWithResult: (MFMailComposeResult) result error: (NSError*) error
{
  RenderWindowOGLIOS* renderWindow = ege_cast<RenderWindowOGLIOS*>(m_object->engine().graphics()->renderTarget(EGE_PRIMARY_RENDER_TARGET_NAME));
  EGE_ASSERT(NULL != renderWindow);
  
  UIWindow* window = renderWindow->window();
  
  [window.rootViewController dismissViewControllerAnimated: YES completion: nil];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

@end
