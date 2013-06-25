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
  
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

@end
