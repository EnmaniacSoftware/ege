#ifndef EGE_CORE_SERVICES_PURCHASESERVICESNULL_H
#define EGE_CORE_SERVICES_PURCHASESERVICESNULL_H

/*! Null implementation for in-game purchases. This is mainly to be used as quick ready-to-use code with no real implementation.
 *  This should not to be used but its own. Platform specific implementations should rather derive from it.
 */

#include "Core/Services/Interface/PurchaseServices.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PurchaseServicesNull : public PurchaseServices
{
  public:
    
    PurchaseServicesNull(Engine& engine);
   ~PurchaseServicesNull();

  public:

    /*! @see PurchaseServices::purchase. */
    EGEResult purchase(const String& product) override;
    /*! @see PurchaseServices::restoreAll. */
    EGEResult restoreAll() override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SERVICES_PURCHASESERVICESNULL_H