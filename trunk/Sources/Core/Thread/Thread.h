#ifndef EGE_CORE_THREAD_H
#define EGE_CORE_THREAD_H

/*! This class represents the seperate thread of control. */

#include <EGE.h>
#include <EGESignal.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Thread : public Object
{
  public:

    Thread();
    virtual ~Thread();

    /* Starts work. */
    virtual EGEResult run();
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_THREAD_H