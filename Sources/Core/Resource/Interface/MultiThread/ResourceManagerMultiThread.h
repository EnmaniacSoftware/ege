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
#include "Core/Resource/Implementation/ResourceManager.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(IResource, PResource)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceManagerMultiThread : public ResourceManager
{
  private:

    /*! Available request types. */
    enum RequestType
    {
      RT_GROUP_LOADED = 0,          /*!< Group has been loaded request. */
      RT_GROUP_LOAD_ERROR,          /*!< Group load error request. */
      RT_PROGRESS                   /*!< Resource processing progress request. */
    };

    /*! Data struct for emission request. */
    struct EmissionRequest
    {
      RequestType type;             /*!< Request type. */

      String groupName;             /*!< Group name. Applies to ET_GROUP_LOADED and ET_GROUP_LOAD_ERROR. */
      u32 count;                    /*!< Number of processed resources. Applies to ET_PROGRESS. */
      u32 total;                    /*!< Number of total resources to process. Applies to ET_PROGRESS. */
    };

    /*! Data struct for processing progress emission request. */
    struct ProcessingProgressRequest
    {
      u32 processed;                /*!< Number of processed resources. */
      u32 total;                    /*!< Total number of resources to process. */
    };

    typedef List<EmissionRequest> EmissionRequestList;

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

    /*! @see ResourceManager::processCommands. */
    void processCommands() override;
    /*! @see ResourceManager::resourceProcessPolicy. */
    ResourceManager::ResourceProcessPolicy resourceProcessPolicy() const override;

    /*! @see EngineModule::construct. */
    EGEResult construct() override;
    /*! @see EngineModule::onShutdown. */
    void onShutdown() override;
    /*! @see EngineModule::update. */
    void update(const Time& time) override;

    /*! Appends given batches for processing. */
    void appendBatchesForProcessing(ProcessingBatchList& batches);
    /*! Processes current batches. */
    void processBatches();
    /*! Adds progress requests for later emission. */
    void addProgressRequest(u32 count, u32 total);
    /*! Adds group loaded request for later emission. */
    void addGroupLoadedRequest(const String& groupName);
    /*! Adds group load error request for later emission. */
    void addGroupLoadErrorRequest(const String& groupName);

  private slots:

    /*! Slot called when work thread terminated its work. */
    void onWorkThreadFinished(const PThread& thread);

  private:

    /*! List of all scheduled groups to load/unload. 
     *  @note This is shared resource.
     */
    ProcessingBatchList m_scheduledList;
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
