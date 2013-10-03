#include "EGEApplication.h"
#include "EGEGraphics.h"
#include "iOS/Graphics/OpenGL/RenderWindowOGLIOS.h"
#import "iOS/Services/GameCenterDelegate.h"

EGE_NAMESPACE

@implementation GameCenterDelegate

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (id) initWithObject: (SocialServicesPrivate*) object
{
  self = [super init];
  if (self)
  {
    self->m_object = object;
  }
  
  return self;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (void) gameCenterViewControllerDidFinish: (GKGameCenterViewController*) gameCenterViewController
{
  // show window
  RenderWindowOGLIOS* renderWindow = ege_cast<RenderWindowOGLIOS*>(m_object->d_func()->app()->graphics()->renderTarget(EGE_PRIMARY_RENDER_TARGET_NAME));
  EGE_ASSERT(NULL != renderWindow);
  
  UIWindow* window = renderWindow->window();
  
  [window.rootViewController dismissViewControllerAnimated: YES completion: nil];
  
  // emit
  emit m_object->d_func()->screenDismissed();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

@end
