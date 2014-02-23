#ifndef EGE_IOS_APPLICATION_APPLICATION_PRIVATE_H
#define EGE_IOS_APPLICATION_APPLICATION_PRIVATE_H

#include "EGE.h"
#include "Core/Event/EventListener.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ApplicationPrivate
{
  public:

    ApplicationPrivate(Application* base);
   ~ApplicationPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(Application)

    /*! Enters main loop. */
    EGEResult run();

  private:
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_IOSAPPLICATION_APPLICATION_PRIVATE_H