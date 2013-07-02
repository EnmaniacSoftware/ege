#ifndef EGE_IOS_SERVICES_PURCHASESERVICES_PRIVATE_H
#define EGE_IOS_SERVICES_PURCHASESERVICES_PRIVATE_H

/*! \brief App Store implementation for social platform. */

#if EGE_PURCHASE_SERVICES_APPSTORE

#include "EGE.h"
#include "EGEList.h"
#include "EGEString.h"
#include "EGEStringList.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PurchaseServices;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PurchaseServicesPrivate
{
  public:
    
    PurchaseServicesPrivate(PurchaseServices* base);
   ~PurchaseServicesPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(PurchaseServices)

  public:

    /*! Constructs object. */
    EGEResult construct();
    /*! PurchaseServices the given product. */
    EGEResult purchase(const String& product);
    /*! Restores all purchases made for so far. */
    EGEResult restoreAll();
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

#endif // EGE_PURCHASE_SERVICES_APPSTORE

#endif // EGE_IOS_SERVICES_PURCHASESERVICES_PRIVATE_H