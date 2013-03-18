#ifndef EGE_CORE_RESOURCEMANAGER_SINGLE_THREAD_PRIVATE_H
#define EGE_CORE_RESOURCEMANAGER_SINGLE_THREAD_PRIVATE_H

#if EGE_RESOURCEMANAGER_SINGLE_THREAD

/*! Single threaded implementation for resource manager class.
 */

#include "EGE.h"
#include "EGEString.h"
#include "EGEXml.h"
#include "EGEMap.h"
#include "EGEList.h"
#include "EGETime.h"
#include "Core/Resource/ResourceManager.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(IResource, PResource)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceManagerPrivate
{
  public:

    ResourceManagerPrivate(ResourceManager* base);
   ~ResourceManagerPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(ResourceManager)

  public:

    /*! Creates object. */
    EGEResult construct();
    /*! Updates object. */
    void update(const Time& time);
    /*! Processes commands. */
    void processCommands();
    /*! Loads group with given name. 
     *  @param name  Group name to be loaded.
     *  @return  Returns EGE_SUCCESS if group has been scheduled for loading. EGE_ERROR_ALREADY_EXISTS if group is already loaded. Otherwise, EGE_ERROR.
     *  @note  Given group, when found, is scheduled for loading rather than loaded immediately.
     */
    EGEResult loadGroup(const String& name);
    /*! Unloads group with given name. */
    void unloadGroup(const String& name);
    /*! Returns resource processing policy. */
    ResourceManager::ResourceProcessPolicy resourceProcessPolicy() const;
    /*! Shuts down. */
    void shutDown();
    /*! Returns current state. */
    ResourceManager::State state() const;

  private:

    /*! Data struct containing information regarding resources to process. */
    struct ProcessingBatch
    {
      bool load;                    /*!< Should resource be loaded. If FALSE resource is to be unloaded. */
      Time startTime;               /*!< Start time of batch processing. */
      StringList groups;            /*!< List of groups to be processed. Last group is the main group. */
      u32 resourcesCount;           /*!< Total number of resource to be processed. */
    };

    typedef List<ProcessingBatch> ProcessingBatchList;

  private slots:

    /*! Slot called when group has been loaded. 
     *  @param Group which has been loaded.
     */
    void onGroupLoaded(const PResourceGroup& group);
    /*! Slot called when group has been unloaded. 
     *  @param Group which has been unloaded.
     */
    void onGroupUnloaded(const PResourceGroup& group);
    /*! Slot called when resource has been loaded. 
     *  @param Resource which has been loaded.
     */
    void onResourceLoaded(const PResource& resource);
    /*! Slot called when resource has been unloaded. 
     *  @param Resource which has been unloaded.
     */
    void onResourceUnloaded(const PResource& resource);

  private:

    /*! List of all pending batches to load/unload. */
    ProcessingBatchList m_processList;
    /*! Current state. */
    ResourceManager::State m_state;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_RESOURCEMANAGER_SINGLE_THREAD

#endif // EGE_CORE_RESOURCEMANAGER_SINGLE_THREAD_PRIVATE_H
