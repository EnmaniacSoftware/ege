#ifndef EGE_CORE_RESOURCEMANAGER_H
#define EGE_CORE_RESOURCEMANAGER_H

/*! Resource manager class. This object is a root to entire resource management.
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
class ResourceManagerPrivate;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceManager : public EngineModule<IResourceManager>
{
  public:

    ResourceManager(Engine& engine);
   ~ResourceManager();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Available resource processing policies. */
    enum ResourceProcessPolicy
    {
      RLP_GROUP = 0,        /*!< Loads enitre group at a time. */
      RLP_RESOURCE          /*!< Loads single resource at a time. */
    };

  public:

    /*! Returns resource processing policy. */
    ResourceProcessPolicy resourceProcessPolicy() const;

  protected slots:

    /*! Slot called at end of the frame. */
    void onFrameEnd();

  private:
    
    /*! @see IResourceManager::addDataDirectory. */
    void addDataDirectory(const String& path) override;
    /*! @see IResourceManager::addResources. */
    EGEResult addResources(String filePath, bool autoDetect = true) override;
    /*! @see IResourceManager::loadGroup. */
    EGEResult loadGroup(const String& name) override;
    /*! @see IResourceManager::unloadGroup. */
    void unloadGroup(const String& name) override;
    /*! @see IResourceManager::loadGroup. */
    PResourceGroup group(const String& name) const override;
    /*! @see IResourceManager::resource. */
    PResource resource(const String& typeName, const String& name, const String& groupName = "") const override;
    /*! @see IResourceManager::materialResource. */
    PResourceMaterial materialResource(const String& name, const String& groupName = "") const override;
    /*! @see IResourceManager::textResource. */
    PResourceText textResource(const String& name, const String& groupName = "") const override;
    /*! @see IResourceManager::soundResource. */
    PResourceSound soundResource(const String& name, const String& groupName = "") const override;
    /*! @see IResourceManager::registerResource. */
    EGEResult registerResource(const String& typeName, egeResourceCreateFunc createFunc) override;
    /*! @see IResourceManager::isResourceRegistered. */
    bool isResourceRegistered(const String& typeName) const override;
    /*! @see IResourceManager::createResource. */
    PResource createResource(const String& typeName, ResourceGroup* group) override;

    /*! Processes the RESOURCES tag.
     *  @param  filePath  relative (with respect to resource root directory) path to resouce file.
     *  @param  tag       resource element to process. 
     */
    EGEResult processResourcesTag(const String& filePath, const PXmlElement& tag);
    /*! Add new group from XML data.
    *   @param  filePath  relative (with respect to resource root directory) path to resouce file containing the group definition.
    *   @param  tag       group element to process. 
    */
    EGEResult addGroup(const String& filePath, const PXmlElement& tag);
    /*! Creates default resources. */
    bool createDefaultResources();
    /*! Destroys default resources. */
    void destroyDefaultResources();
    /*! Processes include command from XML data. 
     *  @param  filePath  relative (with respect to resource root directory) path to resouce file containing the group definition.
     *  @param  tag       include element to process. 
     */
    EGEResult processInclude(const String& filePath, const PXmlElement& tag);
    /*! Builds dependancy list for a given group. */
    bool buildDependacyList(StringList& list, const String& groupName) const;
    /*! Processes commands. */
    void processCommands();
    /*! Unloads all groups. This is called from ResourceManager thread. */
    void unloadAll();
    /*! Returns engine object. */
    Engine& engine() const;

    /*! @see EngineModule::construct. */
    EGEResult construct() override;
    /*! @see EngineModule::uid. */
    u32 uid() const override;
    /*! @see EngineModule::update. */
    void update(const Time& time) override;
    /*! @see EngineModule::onShutdown. */
    void onShutdown() override;

  private:

    /*! Data struct containing registration information for resource. */
    struct ResourceRegistryEntry
    {
      egeResourceCreateFunc m_createFunc;
    };

    typedef List<PResourceGroup> GroupList;

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(ResourceManager)

    /*! Reference to engine. */
    Engine& m_engine;
    /*! List of resource data directories. */
    StringList m_dataDirs;
    /*! Resource groups defined */
    GroupList m_groups;
    /*! Registered resources sorted by type name. */
    Map<String, ResourceRegistryEntry> m_registeredResources;
    /*! Total number of resources to process yet. */
    u32 m_totalResourcesToProcess;
    /*! Number of resources processed so far. */
    u32 m_processedResourcesCount;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCEMANAGER_H
