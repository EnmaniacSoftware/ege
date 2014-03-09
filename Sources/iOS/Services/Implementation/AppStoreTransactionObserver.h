//
//  AppStoreTransactionObserver.h
//  EGE Framework
//
//  Created by enmaniac on 25/06/13.
//  Copyright (c) 2013 Little Bee Studios. All rights reserved.
//

#include "EGE.h"
#include "EGEString.h"
#include "iOS/Services/PurchaseServicesAppStoreIOS_p.h"
#import <StoreKit/StoreKit.h>
#import <Foundation/NSSet.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
@interface AppStoreTransactionObserver : NSObject <SKPaymentTransactionObserver, SKProductsRequestDelegate>
{
  /*! Pointer to associated sound object .*/
  EGE::PurchaseServicesPrivate* m_object;
  /*! Set of known purchaseable items (SKProducts). */
  NSMutableSet* m_purchasableItems;
}

@property (nonatomic, readonly) NSMutableSet* m_purchasableItems;

/*! Initializes object.
 *  @param  object  Object with which initialize object.
 *  @return Created object.
 */
- (id) initWithObject: (EGE::PurchaseServicesPrivate*) object;

/*! Tells an observer that one or more transactions have been updated.
 *  @param  queue         The payment queue that updated the transactions.
 *  @param  transactions  An array of the transactions that were updated.
 */
- (void) paymentQueue: (SKPaymentQueue*) queue updatedTransactions: (NSArray*) transactions;

/*! Called when the App Store responds to the product request.
 *  @param  request   The product request sent to the App Store.
 *  @param  response  Detailed information about the list of products.
 */
- (void) productsRequest: (SKProductsRequest*) request didReceiveResponse: (SKProductsResponse*) response;

/*! Tells the observer that the payment queue has finished sending restored transactions.
 *  @param  queue The payment queue that restored the transactions.
 */
- (void) paymentQueueRestoreCompletedTransactionsFinished: (SKPaymentQueue*) queue;

/*! Tells the observer that an error occurred while restoring transactions.
 *  @param  queue The payment queue that was restoring transactions.
 *  @param  error The error that occurred.
 */
- (void) paymentQueue:(SKPaymentQueue*) queue restoreCompletedTransactionsFailedWithError: (NSError*) error;

/*! Returns purchasable product of a given name or NULL if not found. 
 *  @param  productName Name of the product.
 *  @return Pointer to available product data or NULL if no such product is registered for purchases. 
 */
- (SKProduct*) findProduct: (NSString*) name;

@end
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
