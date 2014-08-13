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
#include "Core/Event/EventListener.h"
#include "Core/ListenerContainer.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(EventManager, PEventManager)
EGE_DECLARE_SMART_CLASS(Object, PObject)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class EventManager : public Object
                   , public ListenerContainer<IEventListener>
{
  public:

    EventManager();
   ~EventManager();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Constructs object. */
    EGEResult construct();
    /*! Updates object. 
     *  @param  time  Time interval since last frame.
     */
    void update(const Time& time);
    /*! Sends event with given ID without any data associated with it. 
     *  @param  id        Event id.
     *  @param  immediate TRUE if even is supposed to be dispatched immediately.
     *  @return EGE_SUCCESS on success.
     */
    EGEResult send(s32 id, bool immediate = false);
    /*! Sends event with given ID and associates integral value. 
     *  @param  id        Event id.
     *  @param  data      Integer associated with event.
     *  @param  immediate TRUE if even is supposed to be dispatched immediately.
     *  @return EGE_SUCCESS on success.
     */
    EGEResult send(s32 id, s32 data, bool immediate = false);
    /*! Sends event with given ID and associates floating value. 
     *  @param  id        Event id.
     *  @param  data      Floating point data associated with event.
     *  @param  immediate TRUE if even is supposed to be dispatched immediately.
     *  @return EGE_SUCCESS on success.
     */
    EGEResult send(s32 id, float32 data, bool immediate = false);
    /*! Sends event with given ID and associates arbitrary data. 
     *  @param  id        Event id.
     *  @param  data      Generic data associated with event.
     *  @param  immediate TRUE if even is supposed to be dispatched immediately.
     *  @return EGE_SUCCESS on success.
     */
    EGEResult send(s32 id, PObject data, bool immediate = false);

  private:

    /*! Event list type. */
    typedef List<PEvent> EventList;

  private:

    /*! Sends event. */
    void notify(PEvent event);
    /*! List of all pending events to send. */
    EventList m_pendingEvents;
    /*! List access mutex. */
    PMutex m_mutex;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_EVENT_EVENTMANAGER_H