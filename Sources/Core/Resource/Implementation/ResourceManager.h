#ifndef EGE_CORE_RESOURCE_RESOURCEMANAGER_H
#define EGE_CORE_RESOURCE_RESOURCEMANAGER_H

/*! Resource manager base class. This object is a root to entire resource management.
 */

#include "EGE.h"
#include "EGEString.h"
#include "EGEXml.h"
#include "EGEMultiMap.h"
#include "EGEList.h"
#include "EGEDynamicArray.h"
#include "EGETime.h"
#include "EGEStringList.h"
#include "Core/Engine/Interface/EngineModule.h"
#include "Core/Resource/Interface/IResourceManager.h"
#include "Core/Resource/Interface/Loader/IResourceLoader.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_PREDECLARE_SMART_CLASS(ResourceGroup, PResourceGroup)
EGE_PREDECLARE_SMART_CLASS(IResource, PResource)
EGE_PREDECLARE_SMART_CLASS(ResourceMaterial, PResourceMaterial)
EGE_PREDECLARE_SMART_CLASS(ResourceText, PResourceText)
EGE_PREDECLARE_SMART_CLASS(ResourceSound, PResourceSound)
EGE_PREDECLARE_SMART_CLASS(Event, PEvent)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef PResource (*egeResourceCreateFunc)(Engine& engine, ResourceGroup* group);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceManager : public EngineModule<IResourceManager>
{
  protected:

    /*! Data struct containing information regarding resources to process. */
    struct ProcessingBatch
    {
      bool load;                    /*!< Should resource be loaded. If FALSE resource is to be unloaded. */
      Time startTime;               /*!< Start time of batch processing. */
      StringList groups;            /*!< List of groups to be processed. Last group is the main group. */
      DynamicArray<PResource> resources;
      s32 resourcesCount;           /*!< Total number of resource to be processed. */
      s32 nextResourceIndex;        /*!< 0-based index of next resource to process for a current (front) group. */
    };

    typedef List<ProcessingBatch> ProcessingBatchList;

  protected:

    /*! Constructor.
     *  @param  engine  Engine object.
     *  @param  loader  Resource loader object. Ownership is transferred to ResourceManager.
     */
    ResourceManager(Engine& engine, IResourceLoader& loader);
    virtual ~ResourceManager();

    /*! Returns engine object. */
    Engine& engine() const;
    
    /*! @see IResourceManager::loadGroup. */
    PResourceGroup group(const String& name) const override;

    /*! @see EngineModule::construct. */
    virtual EGEResult construct() override;
    /*! @see EngineModule::onShutdown. */
    virtual void onShutdown() override;
    /*! @see EngineModule::update. */
    virtual void update(const Time& time) override;

    /*! Builds dependancy list for a given group. */
    bool buildDependacyList(StringList& list, const String& groupName) const;
    /*! Initialized processing batch data for a given group.
     *  @param  batch Batch data to initialize.
     *  @param  groupName Name of the group for which the batch is to be set up.
     *  @return TRUE if successful. Otherwise, FALSE.
     */
    bool initializeProcessingBatch(ProcessingBatch& batch, const String& groupName) const;
    /*! Finalizes processing batch data.
     *  @param  batch Batch data to finalize.
     *  @return TRUE if successful. Otherwise, FALSE.
     */
    bool finalizeProcessingBatch(ProcessingBatch& batch) const;

    /*! Unloads all groups. This is called from ResourceManager thread. */
    void unloadAll();

    /*! Processes next batch. */
    virtual void processBatch();

    // TAGE - rename
    /*! Processes completion of group load opertion.
     *  @param group  Group which has been loaded.
     *  @param result Result of the operation.
     *  @note This method affects m_processList data member. It should should be used with caution after call to this method.
     */
    virtual void onGroupLoaded(const PResourceGroup& group, EGEResult result);
    /*! Processes completion of group unload opertion.
     *  @param group  Group which has been unloaded.
     *  @param result Result of the operation.
     *  @note This method affects m_processList data member. It should should be used with caution after call to this method.
     */
    virtual void onGroupUnloaded(const PResourceGroup& group, EGEResult result);

  protected slots:

    /*! Slot called when resource has been loaded. 
     *  @param resource Resource which has been loaded.
     *  @param result   Result of the operation.
     */
    virtual void onResourceLoaded(const PResource& resource, EGEResult result);
    /*! Slot called when resource has been unloaded. 
     *  @param resource Resource which has been unloaded.
     *  @param result   Result of the operation.
     */
    virtual void onResourceUnloaded(const PResource& resource, EGEResult result);

    /*! Slot called when new group has been created. 
     *  @param group Newly created group.
     */
    void onGroupCreated(const PResourceGroup& group);

  private:
    
    /*! @see IResourceManager::addDataDirectory. */
    void addDataDirectory(const String& path) override;
    /*! @see IResourceManager::addResources. */
    EGEResult addResources(String filePath, bool autoDetect = true) override;
    /*! @see IResourceManager::resource. */
    PResource resource(const String& typeName, const String& name, const String& groupName = "") const override;
    /*! @see IResourceManager::materialResource. */
    PResourceMaterial materialResource(const String& name, const String& groupName = "") const override;
    /*! @see IResourceManager::textResource. */
    PResourceText textResource(const String& name, const String& groupName = "") const override;
    /*! @see IResourceManager::soundResource. */
    PResourceSound soundResource(const String& name, const String& groupName = "") const override;

    /*! Creates default resources. */
    virtual bool createDefaultResources();

    /*! @see EngineModule::uid. */
    u32 uid() const override;

    /*! Loads given resource. 
     *  @param  resource  Resource to be loaded.
     *  @return EGE_SUCCESS if resource has been loaded. EGE_ERROR_ALREADY_EXISTS if resource was already loaded or did not required loading.
     *          EGE_WAIT if resource is still being loaded.
     */
    EGEResult loadResource(PResource& resource) const;
    /*! Unloads given resource. 
     *  @param  resource  Resource to be loaded.
     *  @return EGE_SUCCESS if resource has been unloaded. EGE_ERROR_ALREADY_EXISTS if resource was already unloaded or did not required unloading.
     *          EGE_WAIT if resource is still being unloaded.
     */
    EGEResult unloadResource(PResource& resource) const;

  private:

    typedef List<PResourceGroup> GroupList;

  private: 

    /*! Reference to engine. */
    Engine& m_engine;
    /*! Resource loader. */
    IResourceLoader& m_resourceLoader;

  protected:

    /*! List of resource data directories. */
    StringList m_dataDirs;
    /*! Resource groups defined */
    GroupList m_groups;
    /*! Total number of resources to process yet. */
    s32 m_totalResourcesToProcess;
    /*! Number of resources processed so far. */
    s32 m_processedResourcesCount;
    /*! List of all pending batches to load/unload. */
    ProcessingBatchList m_processList;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCE_RESOURCEMANAGER_H
