#ifndef EGE_WIN32_SERVICES_SOCIALSERVICES_H
#define EGE_WIN32_SERVICES_SOCIALSERVICES_H

/*! Windows implementation of social services. 
 */

#include "EGE.h"
#include "Core/Services/Interface/SocialServicesNull.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SocialServicesWin32 : public SocialServicesNull
{
  public:
    
    SocialServicesWin32(Engine& engine);
   ~SocialServicesWin32();
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_WIN32_SERVICES_SOCIALSERVICES_H