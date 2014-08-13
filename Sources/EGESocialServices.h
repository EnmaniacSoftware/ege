#ifndef EGE_SOCIALSERVICES_H
#define EGE_SOCIALSERVICES_H

#include "Core/Platform.h"
#include "Core/Services/Interface/SocialServices.h"

#ifdef EGE_PLATFORM_WIN32
  #include "Win32/Services/Interface/SocialServicesWin32.h"
#elif EGE_PLATFORM_IOS
  #include "iOS/Services/Interface/SocialServicesIOS.h"
#else
  #error "Implement!"
#endif // EGE_PLATFORM_WIN32

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
extern const char* KSocialServicesDebugName;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_SOCIALSERVICES_H