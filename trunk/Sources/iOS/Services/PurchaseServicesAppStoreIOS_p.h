#ifndef EGE_IOS_SERVICES_PURCHASESERVICES_PRIVATE_H
#define EGE_IOS_SERVICES_PURCHASESERVICES_PRIVATE_H

/*! \brief App Store implementation for social platform. */

#include "EGE.h"
#include "EGEList.h"

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

  private:
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_IOS_SERVICES_PURCHASESERVICES_PRIVATE_H