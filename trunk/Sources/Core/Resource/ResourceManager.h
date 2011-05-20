#ifndef EGE_CORE_RESOURCEMANAGER_H
#define EGE_CORE_RESOURCEMANAGER_H

#include "EGE.h"
#include "EGEString.h"
#include "EGEDataBuffer.h"
#include "EGEXml.h"
#include "Core/Graphics/Material.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(ResourceManager, PResourceManager)
EGE_DECLARE_SMART_CLASS(ResourceGroup, PResourceGroup)
EGE_DECLARE_SMART_CLASS(IResource, PResource)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef PResource (*egeResourceCreateFunc)(Application* app, ResourceManager* manager);
//typedef void      (*egeResourceDestroyFunc)(PResource);

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceManager : public Object
{
  public:

    ResourceManager(Application* app);
    virtual ~ResourceManager();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Sets root directory for resources */
    void setRootDirectory(const EGEString& rootDir);
    /* Adds resources from given file to repository. */
    EGEResult addResources(EGEString fileName);
    /* Adds resources from given buffer. */
   // EGEResult addResources(const PDataBuffer& buffer);
    /* Loads group with given name. */
    EGEResult loadGroup(const EGEString& name);
    /* Unloads group with given name. */
    void unloadGroup(const EGEString& name);
    /* Gets group of the given name. */
    PResourceGroup group(const EGEString& name) const;
    /* Composes full path to given resource */
    EGEString makeFullPath(const EGEString& localPath) const;
    /* Returns resource of a given type and name. */
    PResource resource(const EGEString& typeName, const EGEString& name) const;
    /* Registeres custom resource type. */
    EGEResult registerResource(const EGEString& typeName, egeResourceCreateFunc func);
    /* Returns TRUE if given resource type is registered. */
    bool isResourceRegistered(const EGEString& typeName) const;
    /* Creates instance of resource of the type given by name. */
    PResource createResource(const EGEString& typeName);

  private:
    
    /*! Gets resource root directory */
    inline const EGEString& rootDirectory() const { return m_rootDir; }
    /** Processes the RESOURCES tag.
    *  
    *   \param  filePath  relative (with respect to resource root directory) path to resouce file.
    *   \param  tag       resource element to process. 
    */
    EGEResult processResourcesTag(const EGEString& filePath, const PXmlElement& tag);
    /** Add new group from XML data.
    *  
    *   \param  filePath  relative (with respect to resource root directory) path to resouce file containing the group definition.
    *   \param  tag       group element to process. 
    */
    EGEResult addGroup(const EGEString& filePath, const PXmlElement& tag);

  private:

    /*! Resource root dir */
    EGEString m_rootDir;
    /*! Resource groups defined */
    std::vector<PResourceGroup> m_groups;
    /*! Registered resources sorted by type name. */
    std::map<EGEString, egeResourceCreateFunc> m_registeredResources;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCEMANAGER_H
