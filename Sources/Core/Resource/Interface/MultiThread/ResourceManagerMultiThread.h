#ifndef EGE_CORE_RESOUECE_RESOURCEMANAGER_MULTITHREAD_H
#define EGE_CORE_RESOUECE_RESOURCEMANAGER_MULTITHREAD_H

/*! Multi threaded implementation for resource manager class.
 */

#include "EGE.h"
#include "EGEString.h"
#include "EGEXml.h"
#include "EGEMap.h"
#include "EGEList.h"
#include "EGETime.h"
#include "EGEThread.h"
#include "EGEMutex.h"
#include "EGEWaitCondition.h"
#include "Core/Resource/Implementation/MultiThread/ResourceManagerRequest.h"
#include "Core/Resource/Implementation/ResourceManager.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_PREDECLARE_SMART_CLASS(IResource, PResource)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceManagerMultiThread : public ResourceManager
{
  public:

    ResourceManagerMultiThread(Engine& engine, IResourceLoader& loader);
   ~ResourceManagerMultiThread();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Updates manager. 
     *  @note This is called from worker thread.
     */
    void threadUpdate();

  private:

    /*! @see IResourceManager::loadGroup. */
    EGEResult loadGroup(const String& name) override;
    /*! @see IResourceManager::unloadGroup. */
    void unloadGroup(const String& name) override;

    /*! @see EngineModule::construct. */
    EGEResult construct() override;
    /*! @see EngineModule::onShutdown. */
    void onShutdown() override;
    /*! @see EngineModule::update. */
    void update(const Time& time) override;

    /*! @see ResourceManager::processBatch. */
    void processBatch() override;

    /*! @see ResourceManager::onGroupLoaded. 
     *  @note This override simply schedules processing to main thread where base class implementation should be called from.
     */
    void onGroupLoaded(const PResourceGroup& group, EGEResult result) override;
    /*! @see ResourceManager::onGroupUnloaded. 
     *  @note This override simply schedules processing to main thread where base class implementation should be called from.
     */
    void onGroupUnloaded(const PResourceGroup& group, EGEResult result) override;

    /*! @see ResourceManager::handleResourceLoaded. 
     *  @note This override simply schedules processing to main thread where base class implementation should be called from.
     */
    void handleResourceLoaded(const PResource& resource, EGEResult result, s32 totalResourceCount, s32 processedResourceCount) override;
    /*! @see ResourceManager::handleResourceUnloaded. 
     *  @note This override simply schedules processing to main thread where base class implementation should be called from.
     */
    void handleResourceUnloaded(const PResource& resource, EGEResult result, s32 totalResourceCount, s32 processedResourceCount) override;

  private slots:

    /*! Slot called when work thread terminated its work. */
    void onWorkThreadFinished(const PThread& thread);

  private:

    typedef List<ResourceManagerRequest*> EmissionRequestList;

  private:

    /*! Resource loading/unloading thread. */
    PThread m_workThread;
    /*! Resource data access mutex. */
    PMutex m_mutex;
    /*! Wait condition signaled when any commands are to be processed. */
    PWaitCondition m_commandsToProcess;
    /*! Emit requests pool mutex. */
    PMutex m_emitRequstsMutex;
    /*! Pool of emission requests to send. */
    EmissionRequestList m_emissionRequests;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOUECE_RESOURCEMANAGER_MULTITHREAD_H
