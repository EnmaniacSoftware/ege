//
//  MailComposeDelegate.h
//  EGE Framework
//
//  Created by enmaniac on 7/13/14.
//  Copyright (c) 2014 Little Bee Studios. All rights reserved.
//

#include "EGE.h"
#include "iOS/Services/Interface/DeviceServicesIOS.h"
#import <MessageUI/MFMailComposeViewController.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
@interface MailComposeDelegate : NSObject <MFMailComposeViewControllerDelegate>
{
  /*! Pointer to framework's DeviceServices object .*/
  EGE::DeviceServicesIOS* m_object;
}

/*! Initializes object.
 *  @param  object  Object with which initialize object.
 *  @return Created object.
 */
- (id) initWithObject: (EGE::DeviceServicesIOS*) object;

/*! Called after user dismissed the mail composition view.
 *  @param  controller  The view controller object managing the mail composition view.
 *  @param  result      The result of the user’s action.
 *  @param  error       If an error occurred, this parameter contains an error object with information about the type of failure.
 */
- (void) mailComposeController: (MFMailComposeViewController*) controller didFinishWithResult: (MFMailComposeResult) result error: (NSError*) error;

@end
//--------------------------------------------------------------------------------------------------------------------------------------------------------------