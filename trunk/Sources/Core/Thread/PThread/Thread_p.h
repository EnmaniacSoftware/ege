#ifndef EGE_CORE_THREAD_PRIVATE_H
#define EGE_CORE_THREAD_PRIVATE_H

/*! This class represents the seperate thread of control. */

#include <EGE.h>
#include <EGESignal.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Thread;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ThreadPrivate
{
  public:

    ThreadPrivate(Thread* base);
   ~ThreadPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(Thread)

    /* Starts work. */
    EGEResult run();
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_THREAD_PRIVATE_H