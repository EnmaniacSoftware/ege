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

    /* Locks resources. */
    void lockResources();
    /* Unlocks resources. */
    void unlockResources();

    /*! List of resource data directories. */
    //StringList m_dataDirs;
    ///*! Resource groups defined */
    //GroupList m_groups;
    ///*! Registered resources sorted by type name. */
    //Map<String, ResourceRegistryEntry> m_registeredResources;
    ///*! List of all pending commands to process. */
    //CommandDataList m_commands;
    ///*! Resource loading/unloading thread. */
    //PThread m_workThread;
    ///*! Resource data access mutex. */
    //PMutex m_mutex;
    ///*! Wait condition signaled when any commands are to be processed. */
    //PWaitCondition m_commandsToProcess;
    ///*! Current state. */
    //State m_state;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_RESOURCE_MANAGER_MULTI_THREAD

#endif // EGE_CORE_RESOURCEMANAGER_MULTI_THREAD_PRIVATE_H
