#ifndef EGE_IOS_SERVICES_PURCHASESERVICES_H
#define EGE_IOS_SERVICES_PURCHASESERVICES_H

/*! App Store implementation for in-game purchases. 
 */

#include "EGE.h"
#include "EGEList.h"
#include "EGEString.h"
#include "EGEStringList.h"
#include "Core/Services/Interface/PurchaseServices.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PurchaseServicesIOS : public PurchaseServices
{
  public:
    
    PurchaseServicesIOS(Engine& engine);
   ~PurchaseServicesIOS();

  public:

    /*! @see PurchaseServices::purchase. */
    EGEResult purchase(const String& product) override;
    /*! @see PurchaseServices::restoreAll. */
    EGEResult restoreAll() override;

    /*! Requests processing of next pending purchese. */
    void processNextPurchase();
    /*! Called when purchasing of item is done.
     *  @param  result      Result of purchase.
     *  @param  productName Product purchased.
     */
    void onProductPurchased(EGEResult result, const String& productName);
    /*! Called when restoration of item is done.
     *  @param  result      Result of restoration.
     *  @param  productName Product restored.
     */
    void onProductRestored(EGEResult result, const String& productName);
  
  private:
  
    /*! Returns TRUE if purchasing is enabled. */
    bool isAvailable() const;
  
  private:
  
    /*! Transaction observer. */
    void* m_observer;
    /*! Purchase pending pool. */
    StringList m_pendingPurchases;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_IOS_SERVICES_PURCHASESERVICES_H