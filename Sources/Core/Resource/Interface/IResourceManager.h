#ifndef EGE_CORE_RESOURCE_RESOURCEMANAGER_INTERFACE_H
#define EGE_CORE_RESOURCE_RESOURCEMANAGER_INTERFACE_H

/*! Resource manager interface.
 */

#include "EGE.h"
#include "EGEEngine.h"
#include "EGESignal.h"
#include "EGEString.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(ResourceManager, PResourceManager)
EGE_DECLARE_SMART_CLASS(ResourceGroup, PResourceGroup)
EGE_DECLARE_SMART_CLASS(IResource, PResource)
EGE_DECLARE_SMART_CLASS(ResourceMaterial, PResourceMaterial)
EGE_DECLARE_SMART_CLASS(ResourceText, PResourceText)
EGE_DECLARE_SMART_CLASS(ResourceSound, PResourceSound)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef PResource (*egeResourceCreateFunc)(Engine& engine, ResourceGroup* group);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IResourceManager
{
  public:

    virtual ~IResourceManager() {}

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

    /*! Adds data directory. 
     *  @path path  Location of data directory to be added into pool.
     */
    virtual void addDataDirectory(const String& path) = 0;
    /*! Adds resources from given file to repository. 
     *  @param  filePath    Path to resource definition file which is to be added into resources.
     *  @param  autoDetect  If TRUE file given by filePath will be looked for within currently registered data paths. Otherwise, filePath will be treated as
     *                      absolute path.
     *  @return EGE_SUCCESS on success. Otherwise, one of the available error codes.
     */
    virtual EGEResult addResources(String filePath, bool autoDetect = true) = 0;

    /*! Loads group with given name. 
     *  @param  name  Group name to be loaded.
     *  @return Returns EGE_SUCCESS if group has been scheduled for loading. EGE_ERROR_ALREADY_EXISTS if group is already loaded. Otherwise, EGE_ERROR.
     *  @note Given group, when found, is scheduled for loading rather than loaded immediately.
     */
    virtual EGEResult loadGroup(const String& name) = 0;
    /*! Unloads group with given name. 
     *  @param  name  Group name to unload.
     */
    virtual void unloadGroup(const String& name) = 0;
    /*! Gets group of the given name. 
     *  @param  name  Group name to look for.
     *  @return Found group. NULL if group with given name could not be found.
     */
    virtual PResourceGroup group(const String& name) const = 0;

    /*! Returns resource of a given type and name. Optionally, from given group only. 
     *  @param  typeName  Type of the resource to look for.
     *  @param  name      Name of the resource to look for.
     *  @param  groupName Optional group (if not empty) name within which to look for a resource.
     *  @return Found resource. NULL if resource could not be found.
     */
    virtual PResource resource(const String& typeName, const String& name, const String& groupName = "") const = 0;
    /*! Returns material resource of a given name. Optionally, from given group only. 
     *  @param  name      Resource name to look for.
     *  @param  groupName Optional group (if not empty) name within which to look for a resource.
     *  @return Found resource. NULL if resource could not be found.
     */
    virtual PResourceMaterial materialResource(const String& name, const String& groupName = "") const = 0;
    /*! Returns text resource of a given name. Optionally, from given group only. 
     *  @param  name      Resource name to look for.
     *  @param  groupName Optional group (if not empty) name within which to look for a resource.
     *  @return Found resource. NULL if resource could not be found.
     */
    virtual PResourceText textResource(const String& name, const String& groupName = "") const = 0;
    /*! Returns sound resource of a given name. Optionally, from given group only. 
     *  @param  name      Resource name to look for.
     *  @param  groupName Optional group (if not empty) name within which to look for a resource.
     *  @return Found resource. NULL if resource could not be found.
     */
    virtual PResourceSound soundResource(const String& name, const String& groupName = "") const = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCE_RESOURCEMANAGER_INTERFACE_H
