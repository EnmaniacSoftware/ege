#ifndef EGE_WIN32_DESKTOPKSERVICESPRIVATE_H
#define EGE_WIN32_DESKTOPKSERVICESPRIVATE_H

#include <EGE.h>
#include <EGEString.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class DesktopServices;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class DesktopServicesPrivate
{
  public:

    DesktopServicesPrivate(DesktopServices* base);
   ~DesktopServicesPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS
    
    EGE_DECLARE_PUBLIC_IMPLEMENTATION(DesktopServices)

    /* Opens given URL in Web browser. */
    bool openUrl(const String& url);
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_WIN32_DESKTOPKSERVICESPRIVATE_H