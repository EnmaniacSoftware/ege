#ifndef EGE_CORE_RESOURCE_RESOURCEMANAGER_H
#define EGE_CORE_RESOURCE_RESOURCEMANAGER_H

/*! Resource manager base class. This object is a root to entire resource management.
 */

#include "EGE.h"
#include "EGEString.h"
#include "EGEXml.h"
#include "EGEMap.h"
#include "EGEList.h"
#include "EGETime.h"
#include "EGEStringList.h"
#include "Core/Engine/Interface/EngineModule.h"
#include "Core/Resource/Interface/IResourceManager.h"
#include "Core/Resource/Interface/Loader/IResourceLoader.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(ResourceGroup, PResourceGroup)
EGE_DECLARE_SMART_CLASS(IResource, PResource)
EGE_DECLARE_SMART_CLASS(ResourceMaterial, PResourceMaterial)
EGE_DECLARE_SMART_CLASS(ResourceText, PResourceText)
EGE_DECLARE_SMART_CLASS(ResourceSound, PResourceSound)
EGE_DECLARE_SMART_CLASS(Event, PEvent)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef PResource (*egeResourceCreateFunc)(Engine& engine, ResourceGroup* group);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceManager : public EngineModule<IResourceManager>
{
  public:

    /*! Available resource processing policies. */
    enum ResourceProcessPolicy
    {
      RLP_GROUP = 0,        /*!< Loads enitre group at a time. */
      RLP_RESOURCE          /*!< Loads single resource at a time. */
    };

  public:

    /*! Returns resource processing policy. */
    virtual ResourceProcessPolicy resourceProcessPolicy() const = 0;

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

    /*! Processes commands. */
    virtual void processCommands() = 0;

    /*! Builds dependancy list for a given group. */
    bool buildDependacyList(StringList& list, const String& groupName) const;
    /*! Unloads all groups. This is called from ResourceManager thread. */
    void unloadAll();

  protected slots:

    /*! Slot called at end of the frame. */
    void onFrameEnd();

    /*! Slot called when group has been loaded. 
     *  @param Group which has been loaded.
     */
    virtual void onGroupLoaded(const PResourceGroup& group);
    /*! Slot called when group has been unloaded. 
     *  @param Group which has been unloaded.
     */
    virtual void onGroupUnloaded(const PResourceGroup& group);

    /*! Slot called when resource has been loaded. 
     *  @param Resource which has been loaded.
     */
    virtual void onResourceLoaded(const PResource& resource);
    /*! Slot called when resource has been unloaded. 
     *  @param Resource which has been unloaded.
     */
    virtual void onResourceUnloaded(const PResource& resource);

    /*! Slot called when new group has been created. 
     *  @param group Newly created group.
     */
    void onGroupCreated(const PResourceGroup& group);

  protected:

    /*! Data struct containing information regarding resources to process. */
    struct ProcessingBatch
    {
      bool load;                    /*!< Should resource be loaded. If FALSE resource is to be unloaded. */
      Time startTime;               /*!< Start time of batch processing. */
      StringList groups;            /*!< List of groups to be processed. Last group is the main group. */
      u32 resourcesCount;           /*!< Total number of resource to be processed. */
    };

    typedef List<ProcessingBatch> ProcessingBatchList;

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
    bool createDefaultResources();
    /*! Destroys default resources. */
    void destroyDefaultResources();

    /*! @see EngineModule::uid. */
    u32 uid() const override;

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
    u32 m_totalResourcesToProcess;
    /*! Number of resources processed so far. */
    u32 m_processedResourcesCount;
    /*! List of all pending batches to load/unload. */
    ProcessingBatchList m_processList;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCE_RESOURCEMANAGER_H
