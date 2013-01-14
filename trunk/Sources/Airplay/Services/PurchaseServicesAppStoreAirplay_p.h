#ifndef EGE_AIRPLAY_SERVICES_PURCHASESERVICES_PRIVATE_H
#define EGE_AIRPLAY_SERVICES_PURCHASESERVICES_PRIVATE_H

/*! \brief App Store implementation for social platform. */

#include <EGE.h>
#include <EGEList.h>
#include <s3eIOSAppStoreBilling.h>

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

  private:

    /*! Product information callback. */
    static void ProductInfoCallback(s3eProductInformation* productInfo, void* userData);
    /*! Transaction update callback. */
    static void TransactionUpdateCallback(s3ePaymentTransaction* transaction, void* userData);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_AIRPLAY_SERVICES_PURCHASESERVICES_PRIVATE_H