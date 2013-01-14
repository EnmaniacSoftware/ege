#ifndef EGE_CORE_SERVICES_PURCHASESERVICESS_PRIVATE_H
#define EGE_CORE_SERVICES_PURCHASESERVICESS_PRIVATE_H

/*! \brief Null (mock) implementation for purchases. */

#include <EGE.h>

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
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SERVICES_PURCHASESERVICESS_PRIVATE_H