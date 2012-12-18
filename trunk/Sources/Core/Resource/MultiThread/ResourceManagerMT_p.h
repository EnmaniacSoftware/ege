#ifndef EGE_CORE_RESOURCEMANAGER_MULTI_THREAD_PRIVATE_H
#define EGE_CORE_RESOURCEMANAGER_MULTI_THREAD_PRIVATE_H

#ifdef EGE_RESOURCE_MANAGER_MULTI_THREAD

/*! Single threaded implementation for resource manager class.
 */

#include <EGE.h>
#include <EGEString.h>
#include <EGEXml.h>
#include <EGEMap.h>
#include <EGEList.h>
#include <EGETime.h>
#include <EGEThread.h>
#include <EGEMutex.h>
#include <EGEWaitCondition.h>
#include "Core/Resource/ResourceManager.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(IResource, PResource)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceManagerPrivate
{
  friend class ResourceManagerWorkThread;

  private:

    /*! Data struct containing information regarding resources to process. */
    struct ProcessingBatch
    {
      bool load;                    /*!< Should resource be loaded. If FALSE resource is to be unloaded. */
      String groupName;             /*!< Name of the owning resource group. */
      List<PResource> resources;    /*!< Resources left to be handled. */
    };

    typedef List<ProcessingBatch> ProcessingBatchList;

  public:

    ResourceManagerPrivate(ResourceManager* base);
   ~ResourceManagerPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(ResourceManager)

  public:

    /* Creates object. */
    EGEResult construct();
    /* Updates object. */
    void update(const Time& time);
    /* Updates manager. 
     * @note This is called from worker thread.
     */
    void threadUpdate();
    /* Processes commands. */
    void processCommands();
    /* Loads group with given name. 
     * @param name  Group name to be loaded.
     * @return  Returns EGE_SUCCESS if group has been scheduled for loading. EGE_ERROR_ALREADY_EXISTS if group is already loaded. Otherwise, EGE_ERROR.
     * @note  Given group, when found, is scheduled for loading rather than loaded immediately.
     */
    EGEResult loadGroup(const String& name);
    /* Unloads group with given name. */
    void unloadGroup(const String& name);
    /* Returns resource processing policy. */
    ResourceManager::ResourceProcessPolicy resourceProcessPolicy() const;
    /*! Shuts down. */
    void shutDown();
    /*! Returns current state. */
    ResourceManager::State state() const;

  private:

    /* Appends given batches for processing. */
    void appendBatchesForProcessing(ProcessingBatchList& batches);
    /* Processes current batches. */
    void processBatches();

  private slots:

    /*! Slot called when work thread terminated its work. */
    void onWorkThreadFinished(const PThread& thread);

  private:

    /*! List of all scheduled groups to load/unload. 
     *  @note This is shared resource.
     */
    ProcessingBatchList m_scheduledList;
    /*! List of all pending groups to load/unload. */
    ProcessingBatchList m_pendingList;
    /*! Resource loading/unloading thread. */
    PThread m_workThread;
    /*! Resource data access mutex. */
    PMutex m_mutex;
    /*! Wait condition signaled when any commands are to be processed. */
    PWaitCondition m_commandsToProcess;
    /*! Current state. */
    ResourceManager::State m_state;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_RESOURCE_MANAGER_MULTI_THREAD

#endif // EGE_CORE_RESOURCEMANAGER_MULTI_THREAD_PRIVATE_H
