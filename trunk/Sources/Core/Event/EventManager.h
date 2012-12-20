#ifndef EGE_CORE_EVENTMANAGER_H
#define EGE_CORE_EVENTMANAGER_H

#include <EGE.h>
#include "Core/Event/EventListener.h"
#include "Core/ListenerContainer.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(EventManager, PEventManager)
EGE_DECLARE_SMART_CLASS(Object, PObject)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class EventManager : public Object, public ListenerContainer<IEventListener>
{
  public:

    EventManager(Application* app);
   ~EventManager();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Sends event with given ID. */
    EGEResult send(s32 id);
    /*! Sends event with given ID and associates integral value. */
    EGEResult send(s32 id, s32 data);
    /*! Sends event with given ID and associates floating value. */
    EGEResult send(s32 id, float32 data);
    /*! Sends event with given ID and associates arbitrary data. */
    EGEResult send(s32 id, PObject data);

  private:

    /*! Sends event. */
    void notify(PEvent event);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_EVENTMANAGER_H