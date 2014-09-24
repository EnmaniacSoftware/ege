#ifndef EGE_CORE_ENGINE_ENGINEMODULE_INTERFACE_H
#define EGE_CORE_ENGINE_ENGINEMODULE_INTERFACE_H

/*! Base interface for templated version. This is mainly to be able to contain different template specialization in the same container.
 */

#include "EGE.h"
#include "EGETime.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Available module states. */
enum EngineModuleState
{
  EModuleStateInvalid = 0,
  EModuleStateRunning,
  EModuleStateSuspended,
  EModuleStateShuttingDown,
  EModuleStateClosed
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IEngineModule
{
  public:

    virtual ~IEngineModule() {}

  public:

    /*! Constructs module. 
     *  @return EGE_SUCCESS on success. Otherwise, one of the other error codes.
     */
    virtual EGEResult construct() = 0;

    /*! Returns module UID. */
    virtual u32 uid() const = 0;

    /*! Returns current state. */
    virtual EngineModuleState state() const = 0;

    /*! Called when application is about to be suspended. */
    virtual void onSuspend() = 0;
    /*! Called when application is about to be resumed. */
    virtual void onResume() = 0;
    /*! Called when application is about to be quit. */
    virtual void onShutdown() = 0;

    /*! Updates the module.
     *  @param  time  Time interval since last update.
     */
    virtual void update(const Time& time) = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_ENGINE_ENGINEMODULE_INTERFACE_H