#ifndef EGE_CORE_RESOURCEGROUP_H
#define EGE_CORE_RESOURCEGROUP_H

/** Resource group definition class. This is special object containing delaration of other more specialized resources. Any action on this object is propagated
    to all contained resources ie. when group is loaded, all contained resources are loaded as well.
 */

#include <EGE.h>
#include <EGEXml.h>
#include <EGEString.h>
#include <EGEList.h>
#include <EGEMap.h>
#include <EGESignal.h>
#include "Core/Resource/Resource.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceManager;
EGE_DECLARE_SMART_CLASS(ResourceGroup, PResourceGroup)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceGroup : public Object
{
  public:

    ResourceGroup(Application* app, ResourceManager* manager, const String& name = "");
   ~ResourceGroup();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  signals:

    /*! Signal emitted when resource has been loaded. 
     *  @param resource Resource which has been loaded.
     */
    Signal1<const PResource&> resourceLoaded;
    /*! Signal emitted when resource has been unloaded. 
     *  @param resource Resource which has been unloaded.
     */
    Signal1<const PResource&> resourceUnloaded;
    /*! Signal emitted when resource group has been loaded. 
     *  @param group Resource group which has been loaded.
     */
    Signal1<const PResourceGroup&> resourceGroupLoaded;
    /*! Signal emitted when resource group has been unloaded. 
     *  @param group Resource group which has been unloaded.
     */
    Signal1<const PResourceGroup&> resourceGroupUnloaded;

  public:

    /*! Initializes resource from XML. 
     *  @param  path  full path to resource definition file.
     *  @param  tag   xml element with resource definition. 
     */
    EGEResult create(const String& path,  const PXmlElement& tag);
    /*! Loads the group resources. 
     *  @note When loading is successfully done resourceGroupLoaded signal is emitted. If group was already loaded no singal is emitted.
     *  @return When group is successfully loaded EGE_SUCCESS is returned. If group was already loaded, EGE_ERROR_ALREADY_EXISTS is returned.
     */
    EGEResult load();
    /*! Unloads the group resources. 
     *  @note When unloading is successfully done resourceGroupUnloaded signal is emitted. If group was already unloaded no singal is emitted.
     *  @return When group is successfully unloaded EGE_SUCCESS is returned. If group was already unloaded, EGE_ERROR_ALREADY_EXISTS is returned.
     */
    EGEResult unload();
    /*! Returns group name. */
    const String& name() const { return m_name; }
    /*! Returns resource of a given type and name. */
    PResource resource(const String& typeName, const String& name) const;
    /*! Returns list of all resources of the given type. 
     *  @param Type of the resource to be requested. If empty all resource will be returned.
     */
    List<PResource> resources(const String& typeName) const;
    /*! Returns number of resources. */
    u32 resourceCount() const;
    /*! Adds given resource to group. */
    EGEResult addResource(const PResource& resource);
    /*! Is group loaded. */
    bool isLoaded() const { return m_loaded; }
    /*! Returns list of dependancy group names. */
    const StringList& dependancies() const { return m_dependancies; }
    /*! Gets owning manager */
    ResourceManager* manager() const { return m_manager; }

    /*! Overrides resources by another group ones. 
     *  @param group Group which resources should override current ones.
     *  @note  If current group is not overridable EGE_ERROR_NOT_SUPPORTED is retured. If both groups are the same (same paths)
     *         EGE_ERROR_ALREADY_EXISTS is returned.
     */
    EGEResult overrideBy(const PResourceGroup& group);

  private:

    /*! Destroys group. */
    void destroy();
    /* Adds dependancy. */
    EGEResult addDependancy(const PXmlElement& tag);

  private:

    /*! Container holding all group resources sorted by type name. */
    typedef MultiMap<String, PResource> ResourcesMap;

  private:

    /*! Pointer to owning manager. */
    ResourceManager* m_manager;
    /*! Group name. */
    String m_name;
    /*! Path to definition file. */
    String m_path;
    /*! Resources map. */
    ResourcesMap m_resources;
    /*! Is group loaded. */
    bool m_loaded;
    /*! Dependancy list. */
    StringList m_dependancies;
    /*! Overridable flag. */
    bool m_overridable;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCEGROUP_H
