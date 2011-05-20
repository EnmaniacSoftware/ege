#ifndef EGE_WIN32_APPCONTROLLER_PRIVATE_H
#define EGE_WIN32_APPCONTROLLER_PRIVATE_H

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

    /* Enters main loop. */
    EGEResult run();

  private:

    /*! Pointer to base class. */
    AppController* m_base;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_WIN32_APPCONTROLLER_PRIVATE_H