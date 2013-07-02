#if EGE_PURCHASE_SERVICES_APPSTORE

#include "Core/Services/PurchaseServices.h"
#include "iOS/Services/PurchaseServicesAppStoreIOS_p.h"
#include "EGEMath.h"
#include "EGEMemory.h"
#include "EGEDebug.h"
#import <StoreKit/StoreKit.h>
#import "iOS/Services/AppStoreTransactionObserver.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(PurchaseServicesPrivate)
EGE_DEFINE_DELETE_OPERATORS(PurchaseServicesPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PurchaseServicesPrivate::PurchaseServicesPrivate(PurchaseServices* base) : m_d(base),
                                                                           m_observer(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PurchaseServicesPrivate::~PurchaseServicesPrivate()
{
  [[SKPaymentQueue defaultQueue] removeTransactionObserver: (AppStoreTransactionObserver*) m_observer];
  
  [(AppStoreTransactionObserver*) m_observer release];
  m_observer = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult PurchaseServicesPrivate::construct()
{
  // create transaction observer
  m_observer = [[AppStoreTransactionObserver alloc] initWithObject: this];
  if (nil == m_observer)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }
  
  // register transaction observer
  [[SKPaymentQueue defaultQueue] addTransactionObserver: (AppStoreTransactionObserver*) m_observer];
 
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool PurchaseServicesPrivate::isAvailable() const
{
  return (YES == [SKPaymentQueue canMakePayments]);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult PurchaseServicesPrivate::purchase(const String& product)
{
  EGEResult result = EGE_ERROR;
  
  egeDebug() << "Initiating purchase of" << product;

  if (isAvailable())
  {
    result = EGE_SUCCESS;
    
    // add to pool
    m_pendingPurchases << product;
    
    // check if first item added
    if (1 == m_pendingPurchases.size())
    {
      // process immediately
      processNextPurchase();
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult PurchaseServicesPrivate::restoreAll()
{
  EGEResult result = EGE_ERROR;
  
  egeDebug() << "Initiating purchase restoration";
  
  if (isAvailable())
  {
    result = EGE_SUCCESS;

    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
  }
  
  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PurchaseServicesPrivate::processNextPurchase()
{
  const String product = m_pendingPurchases.front();
  
  // convert product name
  NSString* identifier = [NSString stringWithCString: product.c_str() encoding: NSASCIIStringEncoding];
  
  // check if purchase data is NOT available yet
  SKProduct* registeredProduct = [(AppStoreTransactionObserver*) m_observer findProduct: identifier];
  if (nil == registeredProduct)
  {
    // request purchase data
    SKProductsRequest* request = [[SKProductsRequest alloc] initWithProductIdentifiers: [NSSet setWithObject: identifier]];
    request.delegate = (AppStoreTransactionObserver*) m_observer;
    [request start];
  }
  else
  {
    // purchase
    // NOTE: use SKMutablePayment for quantity > 1
    SKPayment* payment = [SKPayment paymentWithProduct: registeredProduct];
    [[SKPaymentQueue defaultQueue] addPayment: payment];
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PurchaseServicesPrivate::onProductPurchased(EGEResult result, const String& productName)
{
  EGE_ASSERT(m_pendingPurchases.front() == productName);
  
  // remove from pool
  m_pendingPurchases.pop_front();
  
  // emit
  emit d_func()->purchased(result, productName);
  
  // check if more items to purchase
  if ( ! m_pendingPurchases.empty())
  {
    processNextPurchase();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PurchaseServicesPrivate::onProductRestored(EGEResult result, const String& productName)
{
  // emit
  emit d_func()->purchased(result, productName);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_PURCHASE_SERVICES_APPSTORE