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

    Event(Application* app, u32 uid);
    virtual ~Event();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Returns event ID. */
    inline u32 uid() const { return m_uid; }
    /* Sets event data. */
    void setData(PObject data);
    /*! Returns event data. */
    inline PObject data() const { return m_data; }

  private:

    /*! Event ID. */
    u32 m_uid;
    /*! Event data. Can be NULL. */
    PObject m_data;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_EVENT_H