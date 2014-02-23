#include "Core/Services/PurchaseServices.h"
#import "iOS/Services/AppStoreTransactionObserver.h"
#include "EGEPurchaseServices.h"
#include "EGEDebug.h"

EGE_NAMESPACE

@implementation AppStoreTransactionObserver

@synthesize m_purchasableItems;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (id) initWithObject: (PurchaseServicesPrivate*) object
{
  self = [super init];
  if (self)
  {
    // store object pointer
    m_object = object;
  
    // allocate container for items
    m_purchasableItems = [[NSMutableSet setWithCapacity: 5] retain];
  }
  
  return self;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (void) dealloc
{
  [m_purchasableItems release];
  
  [super dealloc];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (void) paymentQueue: (SKPaymentQueue*) queue updatedTransactions: (NSArray*) transactions
{
  for (SKPaymentTransaction* transaction in transactions)
  {
    String productName;
  
    switch (transaction.transactionState)
    {
      case SKPaymentTransactionStatePurchased:
      
        productName = [transaction.payment.productIdentifier UTF8String];
        m_object->onProductPurchased(EGE_SUCCESS, productName);
        break;
        
      case SKPaymentTransactionStateFailed:
        
        productName = [transaction.payment.productIdentifier UTF8String];
        m_object->onProductPurchased(EGE_ERROR, productName);
        break;

      case SKPaymentTransactionStateRestored:
        
        productName = [transaction.payment.productIdentifier UTF8String];
        m_object->onProductRestored(EGE_SUCCESS, productName);
        break;
        
      case SKPaymentTransactionStatePurchasing:
      
        egeDebug(KPurchaseServicesDebugName) << "[AppStoreTransactionObserver] Purchasing:" << [transaction.payment.productIdentifier UTF8String];
        break;
        
      default:

        EGE_ASSERT(false && "Unknown transaction state!");
        break;
    }
    
    // Remove the transaction from the payment queue
    if (SKPaymentTransactionStatePurchasing != transaction.transactionState)
    {
      [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (void) productsRequest: (SKProductsRequest*) request didReceiveResponse: (SKProductsResponse*) response
{
  // add new products into pool
  for (SKProduct* product in response.products)
  {
    [m_purchasableItems addObject: product];
  }
  
  // notify
  m_object->processNextPurchase();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (void) paymentQueueRestoreCompletedTransactionsFinished: (SKPaymentQueue*) queue
{
  // emit
  emit m_object->d_func()->restored(EGE_SUCCESS);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (void) paymentQueue:(SKPaymentQueue*) queue restoreCompletedTransactionsFailedWithError: (NSError*) error
{
  // emit
  emit m_object->d_func()->restored(EGE_ERROR);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
- (SKProduct*) findProduct: (NSString*) name
{
  SKProduct* object = nil;
  
  // add new products into pool
  for (SKProduct* product in m_purchasableItems)
  {
    // check if found
    if (NSOrderedSame == [product.productIdentifier compare: name])
    {
      object = product;
      break;
    }
  }
  
  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

@end
