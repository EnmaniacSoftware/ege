#ifndef EGE_PURCHASESERVICES_H
#define EGE_PURCHASESERVICES_H

#include "Core/Platform.h"
#include "Core/Services/Interface/PurchaseServices.h"

#ifdef EGE_PLATFORM_WIN32
  #include "Win32/Services/Interface/PurchaseServicesWin32.h"
#elif EGE_PLATFORM_IOS
  #include "iOS/Services/Interface/PurchaseServicesIOS.h"
#else
  #error "Implement!"
#endif // EGE_PLATFORM_WIN32

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
extern const char* KPurchaseServicesDebugName;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_PURCHASESERVICES_H