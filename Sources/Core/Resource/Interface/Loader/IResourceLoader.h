#ifndef EGE_CORE_RESOURCE_RESOURCELOADER_INTERFACE_H
#define EGE_CORE_RESOURCE_RESOURCELOADER_INTERFACE_H

/*! Resource loader interface. Loader is an object responsible for accessing data stored in resource files.
 *  Resource files can be provided in the variety of formats ie XML or some binary forms.
 *  This interface defines the common API to deal with it.
 */

#include "EGE.h"
#include "EGESignal.h"
#include "EGEString.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_PREDECLARE_SMART_CLASS(ResourceGroup, PResourceGroup)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IResourceLoader
{
  public:

    virtual ~IResourceLoader() {}

  public signals:

    /*! Signal emitted when new group has been found and created. 
     *  @param group  Instance of the new group.
     */
    Signal1<const PResourceGroup&> signalGroupCreated;

  public:

    /*! Adds resources from given file (source). 
     *  @param  filePath    Path to resource definition file which is to be processed.
     *  @return EGE_SUCCESS on success. EGE_ERROR_NOT_FOUND if resource at given location could not be found. Otherwise, one of the available error codes.
     */
    virtual EGEResult addResources(const String& filePath) = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCE_RESOURCELOADER_INTERFACE_H
