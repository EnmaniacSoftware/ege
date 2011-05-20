#ifndef EGE_CORE_EVENTLISTENER_H
#define EGE_CORE_EVENTLISTENER_H

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(Event, PEvent)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class IEventListener
{
  public:

    /*! Event reciever. */
    virtual void onEventRecieved(PEvent event) = 0;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_EVENTLISTENER_H