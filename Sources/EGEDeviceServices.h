#ifndef EGE_DEVICESERVICES_H
#define EGE_DEVICESERVICES_H

#include "Core/Platform.h"
#include "Core/Services/Interface/DeviceServices.h"
#include "Core/Services/Interface/SpecialURLs.h"

#ifdef EGE_PLATFORM_WIN32
  #include "Win32/Services/Interface/DeviceServicesWin32.h"
#elif EGE_PLATFORM_IOS
  #include "iOS/Services/Interface/DeviceServicesIOS.h"
#else
  #error "Implement!"
#endif // EGE_PLATFORM_WIN32

#endif // EGE_DEVICESERVICES_H