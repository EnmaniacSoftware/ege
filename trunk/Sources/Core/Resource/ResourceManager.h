#ifndef EGE_CORE_RESOURCEMANAGER_H
#define EGE_CORE_RESOURCEMANAGER_H

/** Resource manager class. This object is a root to entire resource management.
 */

#include <EGE.h>
#include <EGEString.h>
#include <EGEXml.h>
#include <EGEMap.h>
#include <EGEList.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(ResourceManager, PResourceManager)
EGE_DECLARE_SMART_CLASS(ResourceGroup, PResourceGroup)
EGE_DECLARE_SMART_CLASS(IResource, PResource)
EGE_DECLARE_SMART_CLASS(ResourceMaterial, PResourceMaterial)
EGE_DECLARE_SMART_CLASS(ResourceText, PResourceText)
EGE_DECLARE_SMART_CLASS(ResourceSound, PResourceSound)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef PResource (*egeResourceCreateFunc)(Application* app, ResourceManager* manager);

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceManager : public Object
{
  public:

    ResourceManager(Application* app);
   ~ResourceManager();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Adds data directory. */
    void addDataDirectory(const String& path);
    /* Adds resources from given file to repository. 
     * @param filePath    Path to resource definition file which is to be added into resources.
     * @param autoDetect  If TRUE file given by filePath will be looked for within currently registered data paths. Otherwise, filePath will be treated as
     *                    absolute path.
     */
    EGEResult addResources(String filePath, bool autoDetect = true);
    /* Adds resources from given buffer. */
   // EGEResult addResources(const PDataBuffer& buffer);
    /* Loads group with given name. */
    EGEResult loadGroup(const String& name);
    /* Unloads group with given name. */
    void unloadGroup(const String& name);
    /* Gets group of the given name. */
    PResourceGroup group(const String& name) const;
    /* Returns resource of a given type and name. Optionally, from given group only. */
    PResource resource(const String& typeName, const String& name, const String& groupName = "") const;
    /* Returns material resource of a given name. Optionally, from given group only. */
    PResourceMaterial materialResource(const String& name, const String& groupName = "") const;
    /* Returns text resource of a given name. Optionally, from given group only. */
    PResourceText textResource(const String& name, const String& groupName = "") const;
    /* Returns sound resource of a given name. Optionally, from given group only. */
    PResourceSound soundResource(const String& name, const String& groupName = "") const;
    /* Registeres custom resource type. */
    EGEResult registerResource(const String& typeName, egeResourceCreateFunc createFunc);
    /* Returns TRUE if given resource type is registered. */
    bool isResourceRegistered(const String& typeName) const;
    /* Creates instance of resource of the type given by name. */
    PResource createResource(const String& typeName);

  private:
    
    /** Processes the RESOURCES tag.
    *  
    *   \param  filePath  relative (with respect to resource root directory) path to resouce file.
    *   \param  tag       resource element to process. 
    */
    EGEResult processResourcesTag(const String& filePath, const PXmlElement& tag);
    /** Add new group from XML data.
    *  
    *   \param  filePath  relative (with respect to resource root directory) path to resouce file containing the group definition.
    *   \param  tag       group element to process. 
    */
    EGEResult addGroup(const String& filePath, const PXmlElement& tag);
    /* Removes all groups. */
    void removeGroups();
    /* Creates default resources. */
    void createDefaultResources();

  private:

    /*! Class containing registration information for resource. */
    class ResourceRegistryEntry
    {
      public:

        egeResourceCreateFunc m_createFunc;
    };

  private:

    typedef List<PResourceGroup> GroupList;

  private:

    /*! List of resource data directories. */
    StringList m_dataDirs;
    /*! Resource groups defined */
    GroupList m_groups;
    /*! Registered resources sorted by type name. */
    Map<String, ResourceRegistryEntry> m_registeredResources;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCEMANAGER_H
