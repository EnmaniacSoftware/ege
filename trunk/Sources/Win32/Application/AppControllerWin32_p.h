#ifndef EGE_WIN32_APPLICATION_APPLICATION_PRIVATE_H
#define EGE_WIN32_APPLICATION_APPLICATION_PRIVATE_H

#include <EGE.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Application;
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
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_WIN32_APPLICATION_APPLICATION_PRIVATE_H