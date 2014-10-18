#ifndef EGE_CORE_ENGINE_ENGINEINTERNAL_H
#define EGE_CORE_ENGINE_ENGINEINTERNAL_H

/*! Framework's internal interface of an engine implementation.  
 */

#include "EGE.h"
#include "EGEDictionary.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Available application states. */
enum EngineState
{
  EStateInvalid = 0,
  EStateRunning,
  EStatePaused,
  EStateQuitting,
  EStateClosed
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AdNetworkRegistry;
class AdNetwork;
class AudioManagerFactory;
class ResourceManagerFactory;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class EngineInternal
{
  public:

    virtual ~EngineInternal() {}

  public:

    /*! Updates engine. */
    virtual void update() = 0;
    /*! Renders frame. */
    virtual void render() = 0;
    
    /*! Returns pointer to ad network registry. */
    virtual AdNetworkRegistry* adNetworkRegistry() const = 0;
    /*! Returns pointer to available ad network solution. */
    virtual AdNetwork* adNetwork() const = 0;
    /*! Returns pointer to audio manager factory. */
    virtual AudioManagerFactory* audioManagerFactory() const = 0;
    /*! Returns pointer to resource manager factory. */
    virtual ResourceManagerFactory* resourceManagerFactory() const = 0;

    /*! Requests engine shutdown. */
    virtual void shutdown() = 0;
    /*! Returns TRUE if engine is in shutting down sequence. */
    virtual bool isShuttingDown() const = 0;
    /*! Returns TRUE if engine is shut down. */
    virtual bool isShutDown() const = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_ENGINE_ENGINEINTERNAL_H