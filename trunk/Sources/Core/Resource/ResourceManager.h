#ifndef EGE_CORE_RESOURCEMANAGER_H
#define EGE_CORE_RESOURCEMANAGER_H

#include <EGE.h>
#include <EGEString.h>
#include <EGEDataBuffer.h>
#include <EGEXml.h>
#include <EGEMap.h>
#include <EGEList.h>
#include "Core/Graphics/Material.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(ResourceManager, PResourceManager)
EGE_DECLARE_SMART_CLASS(ResourceGroup, PResourceGroup)
EGE_DECLARE_SMART_CLASS(IResource, PResource)

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
    /* Sets root directory for resources */
    void setRootDirectory(const String& rootDir);
    /* Adds resources from given file to repository. */
    EGEResult addResources(String fileName);
    /* Adds resources from given buffer. */
   // EGEResult addResources(const PDataBuffer& buffer);
    /* Loads group with given name. */
    EGEResult loadGroup(const String& name);
    /* Unloads group with given name. */
    void unloadGroup(const String& name);
    /* Gets group of the given name. */
    PResourceGroup group(const String& name) const;
    /* Composes full path to given resource */
    String makeFullPath(const String& localPath) const;
    /* Returns resource of a given type and name. Optionally, from given group only. */
    PResource resource(const String& typeName, const String& name, const String& groupName = "") const;
    /* Registeres custom resource type. */
    EGEResult registerResource(const String& typeName, egeResourceCreateFunc createFunc);
    /* Returns TRUE if given resource type is registered. */
    bool isResourceRegistered(const String& typeName) const;
    /* Creates instance of resource of the type given by name. */
    PResource createResource(const String& typeName);

  private:
    
    /*! Gets resource root directory */
    inline const String& rootDirectory() const { return m_rootDir; }
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

  private:

    /*! Class containing registration information for resource. */
    class ResourceRegistryEntry
    {
      public:

        egeResourceCreateFunc m_createFunc;
    };

  private:

    /*! Resource root dir */
    String m_rootDir;
    /*! Resource groups defined */
    List<PResourceGroup> m_groups;
    /*! Registered resources sorted by type name. */
    Map<String, ResourceRegistryEntry> m_registeredResources;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCEMANAGER_H
