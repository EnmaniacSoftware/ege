#ifndef EGE_CORE_DESKTOPKSERVICES_H
#define EGE_CORE_DESKTOPKSERVICES_H

#include <EGE.h>
#include <EGEString.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class DesktopServices
{
  public:

    DesktopServices();
   ~DesktopServices();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS
    
    /*! Creates object. */
    EGEResult construct();
    /*! Opens given URL in Web browser. */
    bool openUrl(const String& url);

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(DesktopServices);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DESKTOPKSERVICES_H