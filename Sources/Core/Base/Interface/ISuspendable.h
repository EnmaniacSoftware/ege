#ifndef EGE_CORE_BASE_SUSPENDABLE_INTERFACE_H
#define EGE_CORE_BASE_SUSPENDABLE_INTERFACE_H

/*! Interface for suspendable objects i.e. objects whose execution can be paused/resumed over time. */

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ISuspendable
{
  public:

    virtual ~ISuspendable() {}

  public:

    /*! Called when object execution should be suspended. */
    virtual void suspend() = 0;
    /*! Called when object execution should be resumed. */
    virtual void resume() = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_BASE_SUSPENDABLE_INTERFACE_H