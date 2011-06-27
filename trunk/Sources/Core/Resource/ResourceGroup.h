#ifndef EGE_CORE_RESOURCEGROUP_H
#define EGE_CORE_RESOURCEGROUP_H

#include <EGE.h>
#include <EGEXml.h>
#include <EGEString.h>
#include <EGEList.h>
#include <EGEMap.h>
#include "Core/Resource/Resource.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceManager;

EGE_DECLARE_SMART_CLASS(ResourceGroup, PResourceGroup)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceGroup : public Object
{
  public:

    ResourceGroup(Application* app, ResourceManager* manager, const EGEString& path);
   ~ResourceGroup();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Initializes group from XML. */
    EGEResult create(const PXmlElement& tag);
    /* Loads the group resources. */
    EGEResult load();
    /* Unloads the group resources. */
    void unload();
    /*! Returns group name. */
    inline const EGEString& name() const { return m_name; }
    /* Returns resource of a given type and name. */
    PResource resource(const EGEString& typeName, const EGEString& name) const;
    /* Returns list of all resources of the given type. */
    EGEList<PResource> resources(const EGEString& typeName) const;
    /*! Returns path to location of group file. */
    inline const EGEString& path() const { return m_path; }

  private:

    /*! Is group loaded. */
    inline bool isLoaded() const { return m_loaded; }
    /*! Gets owning manager */
    inline ResourceManager* manager() const { return m_manager; }
    /* Destroys group. */
    void destroy();

  private:

    /*! Container holding all group resources sorted by type name. */
    typedef EGEMultiMap<EGEString, PResource> ResourcesMap;

  private:

    /*! Pointer to owning manager. */
    ResourceManager* m_manager;
    /*! Group name. */
    EGEString m_name;
    /*! Resources map. */
    ResourcesMap m_resources;
    /*! Is group loaded. */
    bool m_loaded;
    /*! Relative (with respect to root resource directory) path to group definition. */
    EGEString m_path;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCEGROUP_H
