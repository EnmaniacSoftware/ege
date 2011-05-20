#ifndef EGE_AIRPLAY_APPCONTROLLER_PRIVATE_H
#define EGE_AIRPLAY_APPCONTROLLER_PRIVATE_H

#include "EGE.h"
#include "Core/Event/EventListener.h"
#include "Core/ConfigParams.h"
#include "EGETime.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class AppControllerPrivate
{
  public:

    AppControllerPrivate(AppController* base);
   ~AppControllerPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(AppController)

    /* Enters main loop. */
    EGEResult run();
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_AIRPLAY_APPCONTROLLER_PRIVATE_H