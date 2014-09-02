#include "iOS/Services/Interface/AdNetworkChartboost.h"

#import "iOS/Services/Implementation/AdNetworkChartboostDelegate.h"

EGE_NAMESPACE

@implementation AdNetworkChartboostDelegate

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (id) initWithObject: (AdNetworkChartboost*) object
{
  self = [super init];
  if (self)
  {
    self->m_object = object;
  }
  
  return self;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

@end
