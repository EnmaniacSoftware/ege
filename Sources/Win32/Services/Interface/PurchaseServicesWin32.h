#ifndef EGE_WIN32_SERVICES_PURCHASESERVICES_H
#define EGE_WIN32_SERVICES_PURCHASESERVICES_H

/*! Windows implementation for in-game purchases. 
 */

#include "Core/Services/Interface/PurchaseServicesNull.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PurchaseServicesWin32 : public PurchaseServicesNull
{
  public:
    
    PurchaseServicesWin32(Engine& engine);
   ~PurchaseServicesWin32();
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_WIN32_SERVICES_PURCHASESERVICES_H