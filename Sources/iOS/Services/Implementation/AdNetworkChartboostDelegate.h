//
//  ChartboostDelegate.h
//  EGE Framework
//
//  Created by enmaniac on 17/06/13.
//  Copyright (c) 2013 Little Bee Studios. All rights reserved.
//

#include "EGE.h"
#include "iOS/Services/Interface/AdNetworkChartboost.h"
#import "Chartboost.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
@interface AdNetworkChartboostDelegate : NSObject <ChartboostDelegate>
{
  /*! Pointer to framework's AdNetworkCharboost object .*/
  EGE::AdNetworkChartboost* m_object;
}

/*! Initializes object.
 *  @param  object  Object with which initialize object.
 *  @return Created object.
 */
- (id) initWithObject: (EGE::AdNetworkChartboost*) object;

@end
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
