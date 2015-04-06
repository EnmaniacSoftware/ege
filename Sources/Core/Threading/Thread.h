#ifndef EGE_CORE_THREAD_H
#define EGE_CORE_THREAD_H

/*! This class represents the seperate thread of control. */

#include "EGE.h"
#include "EGEAtomicBool.h"
#include "EGESignal.h"
#include "EGETime.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_PREDECLARE_SMART_CLASS(Thread, PThread)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Thread : public Object
{
  public:

    Thread();
    virtual ~Thread();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public signals:

    /*! Signal emitted when thread finished its work. 
     *  @note Signal is emitted from finished thread. 
     */
    Signal1<const PThread&> finished;
    /*! Signal emitted when thread started its work. 
     *  @note Signal is emitted from started thread. 
     */
    Signal1<const PThread&> started;

  public:

    /*! Returns current tread identifier. */
    static void* CurrentId();

  public:

    /*! Returns TRUE if object is valid. */
    bool isValid() const;

    /*! Starts thread. */
    bool start();
    /*! Stops thread. 
     *  @param  exitCode  Code the thread is going to exit with.
     *  @note This exists the thread in a gently way. This is only possible if a thread is checking regularily the value is isStopping method.
     *        Once this method returns TRUE, thread function should return.
     */
    void stop(s32 exitCode = 0);
    /*! Cancels tread. 
     *  @note This forcibly stops the thread. Use with caution.
     */
    void cancel();
    
    /*! Waits for thread to be finished. */
    bool wait();

    /*! Returns TRUE if thread is currently running. */
    bool isRunning() const;
    /*! Returns TRUE if thread has finished its work. */
    bool isFinished() const;
    /*! Returns TRUE if thread has been requested to terminate. */
    bool isStopping() const;

  protected:

    /*! Work method. */
    virtual s32 run() = 0;

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(Thread);

    /*! Stop request flag. */
    mutable AtomicBool m_stopping;
    /*! Exit code. Used with explicit stopping. */
    s32 m_exitCode;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_THREAD_H