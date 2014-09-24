#ifndef EGE_CORE_EVENT_EVENTMANAGER_INTERFACE_H
#define EGE_CORE_EVENT_EVENTMANAGER_INTERFACE_H

/*! Public interface allowing sending the events to the rest of engine.
 *  In normal circumstances, all events are deliver at the beging of the a frame.
 *  It is possible, however, to ask manager to dispatch event immediately.
 *  Event manager is tread safe.
 */

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IEventListener;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IEventManager 
{
  public:

    virtual ~IEventManager() {}

  public:

    /*! Registers the listener for notifications.
     *  @param  listener  Listener object to register for events.
     *  @return TRUE if listener has been successfully registered.
     */
    virtual bool registerListener(IEventListener* listener) = 0;
    /*! Unregisters the listener from notifications.
     *  @param  listener  Listener object to unregister.
     */
    virtual void unregisterListener(IEventListener* listener) = 0;

    /*! Sends event with given ID without any data associated with it. 
     *  @param  id        Event id.
     *  @param  immediate TRUE if even is supposed to be dispatched immediately.
     *  @return EGE_SUCCESS on success.
     */
    virtual EGEResult send(s32 id, bool immediate = false) = 0;
    /*! Sends event with given ID and associates integral value. 
     *  @param  id        Event id.
     *  @param  data      Integer associated with event.
     *  @param  immediate TRUE if even is supposed to be dispatched immediately.
     *  @return EGE_SUCCESS on success.
     */
    virtual EGEResult send(s32 id, s32 data, bool immediate = false) = 0;
    /*! Sends event with given ID and associates floating value. 
     *  @param  id        Event id.
     *  @param  data      Floating point data associated with event.
     *  @param  immediate TRUE if even is supposed to be dispatched immediately.
     *  @return EGE_SUCCESS on success.
     */
    virtual EGEResult send(s32 id, float32 data, bool immediate = false) = 0;
    /*! Sends event with given ID and associates arbitrary data. 
     *  @param  id        Event id.
     *  @param  data      Generic data associated with event.
     *  @param  immediate TRUE if even is supposed to be dispatched immediately.
     *  @return EGE_SUCCESS on success.
     */
    virtual EGEResult send(s32 id, PObject data, bool immediate = false) = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_EVENT_EVENTMANAGER_INTERFACE_H