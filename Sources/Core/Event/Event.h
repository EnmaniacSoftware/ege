#ifndef EGE_CORE_EVENT_H
#define EGE_CORE_EVENT_H

#include "EGE.h"
#include "Core/Event/EventIDs.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Event, PEvent)
EGE_DECLARE_SMART_CLASS(Object, PObject)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Event : public Object
{
  public:

    Event(s32 id);
    virtual ~Event();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Returns event ID. */
    s32 id() const;
    /*1 Sets event data. */
    void setData(PObject data);
    /*! Returns event data. */
    PObject data() const;

  private:

    /*! Event ID. */
    s32 m_id;
    /*! Event data. Can be NULL. */
    PObject m_data;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_EVENT_H