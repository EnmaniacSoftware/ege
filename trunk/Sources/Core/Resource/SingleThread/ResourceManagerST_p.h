#ifndef EGE_CORE_RESOURCEMANAGER_SINGLE_THREAD_PRIVATE_H
#define EGE_CORE_RESOURCEMANAGER_SINGLE_THREAD_PRIVATE_H

#ifdef EGE_RESOURCE_MANAGER_SINGLE_THREAD

/*! Single threaded implementation for resource manager class.
 */

#include <EGE.h>
#include <EGEString.h>
#include <EGEXml.h>
#include <EGEMap.h>
#include <EGEList.h>
#include <EGETime.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceManager;
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

    /* Creates object. */
    EGEResult construct();
    /* Updates object. */
    void update(const Time& time);
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

  private:

    /*! Data struct containing information regarding resources to process. */
    struct ProcessingBatch
    {
      bool load;                    /*!< Should resource be loaded. If FALSE resource is to be unloaded. */
      String groupName;             /*!< Name of the owning resource group. */
      List<PResource> resources;    /*!< Resources left to be handled. */
    };

    typedef List<ProcessingBatch> ProcessingBatchList;

  private:

    /*! List of all pending batches to load/unload. */
    ProcessingBatchList m_processList;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_RESOURCE_MANAGER_SINGLE_THREAD

#endif // EGE_CORE_RESOURCEMANAGER_SINGLE_THREAD_PRIVATE_H
