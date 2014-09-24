#ifndef EGE_CORE_EVENT_EVENTMANAGER_H
#define EGE_CORE_EVENT_EVENTMANAGER_H

/** Event manager is responsible for delivery of events to any objects which subscribed for it.
 *  In normal circumstances, all events are deliver at the beging of the a frame.
 *  It is possible, however, to ask manager to dispatch event immediately.
 *  Event manager is tread safe.
 */

#include "EGE.h"
#include "EGEList.h"
#include "EGEMutex.h"
#include "EGETime.h"
#include "Core/Event/Interface/IEventManager.h"
#include "Core/Event/EventListener.h"
#include "Core/ListenerContainer.h"
#include "Core/Engine/Interface/EngineModule.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Object, PObject)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class EventManager : public EngineModule<IEventManager>
                   , public ListenerContainer<IEventListener>

{
  public:

    EventManager();
   ~EventManager();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  private:

    /*! @see IEventManager::registerListener. */
    bool registerListener(IEventListener* listener) override;
    /*! @see IEventManager::unregisterListener. */
    void unregisterListener(IEventListener* listener) override;
    /*! @see IEventManager::send. */
    EGEResult send(s32 id, bool immediate = false) override;
    /*! @see IEventManager::send. */
    EGEResult send(s32 id, s32 data, bool immediate = false) override;
    /*! @see IEventManager::send. */
    EGEResult send(s32 id, float32 data, bool immediate = false) override;
    /*! @see IEventManager::send. */
    EGEResult send(s32 id, PObject data, bool immediate = false) override;

  private:

    /*! @see EngineModule::uid. */
    u32 uid() const override;
    /*! @see EngineModule::update. */
    void update(const Time& time) override;

    /*! Sends event. */
    void notify(PEvent event);

  private:

    /*! Event list type. */
    typedef List<PEvent> EventList;

  private:

    /*! List of all pending events to send. */
    EventList m_pendingEvents;
    /*! List access mutex. */
    PMutex m_mutex;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_EVENT_EVENTMANAGER_H