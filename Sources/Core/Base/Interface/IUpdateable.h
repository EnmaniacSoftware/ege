#ifndef EGE_CORE_BASE_UPDATEABLE_INTERFACE_H
#define EGE_CORE_BASE_UPDATEABLE_INTERFACE_H

/*! Interface for updateable objects i.e. objects whose state changes over time. */

#include "EGETime.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IUpdateable
{
  public:

    virtual ~IUpdateable() {}

  public:

    /*! Updates object.
     *  @param  time  Time duration.
     */
    virtual void update(const Time& time) = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_BASE_UPDATEABLE_INTERFACE_H