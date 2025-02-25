#ifndef EGE_CORE_RESOURCEMANAGER_H
#define EGE_CORE_RESOURCEMANAGER_H

/*! Resource manager class. This object is a root to entire resource management.
 */

#include "EGE.h"
#include "EGEString.h"
#include "EGEXml.h"
#include "EGEMap.h"
#include "EGEList.h"
#include "EGESignal.h"
#include "EGETime.h"
#include "EGEStringList.h"
#include "Core/Event/EventListener.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(ResourceManager, PResourceManager)
EGE_DECLARE_SMART_CLASS(ResourceGroup, PResourceGroup)
EGE_DECLARE_SMART_CLASS(IResource, PResource)
EGE_DECLARE_SMART_CLASS(ResourceMaterial, PResourceMaterial)
EGE_DECLARE_SMART_CLASS(ResourceText, PResourceText)
EGE_DECLARE_SMART_CLASS(ResourceSound, PResourceSound)
EGE_DECLARE_SMART_CLASS(Event, PEvent)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef PResource (*egeResourceCreateFunc)(Application* app, ResourceGroup* group);
class ResourceManagerPrivate;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceManager : public Object, public IEventListener
{
  public:

    ResourceManager(Application* app);
   ~ResourceManager();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public signals:

    /*! Signal emitted when group has been loaded. 
     *  @param name Name of a group which has been loaded.
     *  @note Signal is emitted in manager's thread. 
     */
    Signal1<const String&> groupLoadComplete;
    /*! Signal emitted when group could not be loaded. 
     *  @param name Name of a group which failed to load.
     *  @note Signal is emitted in manager's thread. 
     */
    Signal1<const String&> groupLoadError;
    /*! Signal emitted when resource has been processed.
     *  @param processed Number of already processed resources
     *  @param total     Total number resources to process
     *  @note Signal is NOT guaranteed to be emitted in manager's thread. 
     */
    Signal2<u32, u32> processingStatusUpdated;

  public:

    /*! Available states. */
    enum State
    {
      STATE_NONE = -1,      /*!< Uninitialized. */
      STATE_READY,          /*!< Ready to use. */
      STATE_CLOSING,        /*!< Closing. */
      STATE_CLOSED          /*!< Closed. */
    };

    /*! Available resource processing policies. */
    enum ResourceProcessPolicy
    {
      RLP_GROUP = 0,        /*!< Loads enitre group at a time. */
      RLP_RESOURCE          /*!< Loads single resource at a time. */
    };

  public:

    /*! Creates object. */
    EGEResult construct();
    /*! Returns current state. */
    State state() const;
    /* Updates object. */
    void update(const Time& time);
    /*! Adds data directory. */
    void addDataDirectory(const String& path);
    /*! Adds resources from given file to repository. 
     *  @param filePath    Path to resource definition file which is to be added into resources.
     *  @param autoDetect  If TRUE file given by filePath will be looked for within currently registered data paths. Otherwise, filePath will be treated as
     *                     absolute path.
     */
    EGEResult addResources(String filePath, bool autoDetect = true);
    /*! Loads group with given name. 
     *  @param name  Group name to be loaded.
     *  @return  Returns EGE_SUCCESS if group has been scheduled for loading. EGE_ERROR_ALREADY_EXISTS if group is already loaded. Otherwise, EGE_ERROR.
     *  @note  Given group, when found, is scheduled for loading rather than loaded immediately.
     */
    EGEResult loadGroup(const String& name);
    /*! Unloads group with given name. */
    void unloadGroup(const String& name);
    /*! Gets group of the given name. */
    PResourceGroup group(const String& name) const;
    /*! Returns resource of a given type and name. Optionally, from given group only. */
    PResource resource(const String& typeName, const String& name, const String& groupName = "") const;
    /*! Returns material resource of a given name. Optionally, from given group only. */
    PResourceMaterial materialResource(const String& name, const String& groupName = "") const;
    /*! Returns text resource of a given name. Optionally, from given group only. */
    PResourceText textResource(const String& name, const String& groupName = "") const;
    /*! Returns sound resource of a given name. Optionally, from given group only. */
    PResourceSound soundResource(const String& name, const String& groupName = "") const;
    /*! Registeres custom resource type. */
    EGEResult registerResource(const String& typeName, egeResourceCreateFunc createFunc);
    /*! Returns TRUE if given resource type is registered. */
    bool isResourceRegistered(const String& typeName) const;
    /*! Creates instance of resource of the type given by name. */
    PResource createResource(const String& typeName, ResourceGroup* group);
    /*! Returns resource processing policy. */
    ResourceProcessPolicy resourceProcessPolicy() const;

  protected slots:

    /*! Slot called at end of the frame. */
    void onFrameEnd();

  private:
    
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
    /*! @see IEventListener::onEventRecieved. */
    void onEventRecieved(PEvent event) override;
    /*! Shuts down. */
    void shutDown();
    /*! Processes commands. */
    void processCommands();
    /*! Unloads all groups. This is called from ResourceManager thread. */
    void unloadAll();

  private:

    /*! Data struct containing registration information for resource. */
    struct ResourceRegistryEntry
    {
      egeResourceCreateFunc m_createFunc;
    };

    typedef List<PResourceGroup> GroupList;

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(ResourceManager)

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
