#ifndef EGE_AIRPLAY_APPLICATION_APPLICATION_PRIVATE_H
#define EGE_AIRPLAY_APPLICATION_APPLICATION_PRIVATE_H

#include <EGE.h>
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

    /*! Pause callback. */
    static int32 PauseCB(void* data, void* userData);
    /*! Resume callback. */
    static int32 ResumeCB(void* data, void* userData);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_AIRPLAY_APPLICATION_APPLICATION_PRIVATE_H