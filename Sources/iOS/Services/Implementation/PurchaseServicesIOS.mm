#include "iOS/Services/Interface/PurchaseServicesIOS.h"
#include "EGEMath.h"
#include "EGEMemory.h"
#include "EGEPurchaseServices.h"
#include "EGEDebug.h"
#import <StoreKit/StoreKit.h>
#import "iOS/Services/Implementation/AppStoreTransactionObserver.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
extern const char* KPurchaseServicesDebugName;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PurchaseServicesIOS::PurchaseServicesIOS(Engine& engine)
: PurchaseServices(engine)
, m_observer(NULL)
{
  // create transaction observer
  m_observer = [[AppStoreTransactionObserver alloc] initWithObject: this];
  
  // register transaction observer
  [[SKPaymentQueue defaultQueue] addTransactionObserver: (AppStoreTransactionObserver*) m_observer];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PurchaseServicesIOS::~PurchaseServicesIOS()
{
  [[SKPaymentQueue defaultQueue] removeTransactionObserver: (AppStoreTransactionObserver*) m_observer];
  
  [(AppStoreTransactionObserver*) m_observer release];
  m_observer = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool PurchaseServicesIOS::isAvailable() const
{
  return (YES == [SKPaymentQueue canMakePayments]);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult PurchaseServicesIOS::purchase(const String& product)
{
  EGEResult result = EGE_ERROR;
  
  egeDebug(KPurchaseServicesDebugName) << "Initiating purchase of" << product;

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
EGEResult PurchaseServicesIOS::restoreAll()
{
  EGEResult result = EGE_ERROR;
  
  egeDebug(KPurchaseServicesDebugName) << "Initiating purchase restoration";
  
  if (isAvailable())
  {
    result = EGE_SUCCESS;

    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
  }
  
  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PurchaseServicesIOS::processNextPurchase()
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
void PurchaseServicesIOS::onProductPurchased(EGEResult result, const String& productName)
{
  EGE_ASSERT(m_pendingPurchases.front() == productName);
  
  // remove from pool
  m_pendingPurchases.pop_front();
  
  // emit
  emit purchased(result, productName);
  
  // check if more items to purchase
  if ( ! m_pendingPurchases.empty())
  {
    processNextPurchase();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PurchaseServicesIOS::onProductRestored(EGEResult result, const String& productName)
{
  // emit
  emit purchased(result, productName);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
