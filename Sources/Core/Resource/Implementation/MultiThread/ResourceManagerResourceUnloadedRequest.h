#ifndef EGE_CORE_RESOURCE_MULTITHREAD_RESOURCEMANAGERRESOURCEUNLOADEDREQUEST_H
#define EGE_CORE_RESOURCE_MULTITHREAD_RESOURCEMANAGERRESOURCEUNLOADEDREQUEST_H

/*! Multithreaded resource manager's resource loaded request type. 
 *  This request represent completion of single resource loading.
 */

#include "EGE.h"
#include "Core/Resource/Implementation/MultiThread/ResourceManagerRequest.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_PREDECLARE_SMART_CLASS(IResource, PResource)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceManagerResourceUnloadedRequest : public ResourceManagerRequest
{
  public:

    ResourceManagerResourceUnloadedRequest(const PResource& resource, EGEResult result, s32 count, s32 total) 
    : m_resource(resource)
    , m_result(result)
    , m_totalResourcesToProcess(total) 
    , m_processedResourcesCount(count)
    {}

    /*! Returns resource. */
    const PResource& resource() const { return m_resource; }
    /*! Returns request result. */
    EGEResult result() const { return m_result; }
    /*! Returns total number of resources to process yet. */
    s32 totalResourceCount() const { return m_totalResourcesToProcess; }
    /*! Returns number of resources processed so far. */
    s32 processedResourceCount() const { return m_processedResourcesCount; }

  private:

    /*! @see ResourceManagerRequest::type. */
    RequestType type() const override { return EResourceUnloaded; }

  private:

    /*! Resource. */
    PResource m_resource;
    /*! Result. */
    EGEResult m_result;
    /*! Total number of resources to process yet. */
    s32 m_totalResourcesToProcess;
    /*! Number of resources processed so far. */
    s32 m_processedResourcesCount;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCE_MULTITHREAD_RESOURCEMANAGERRESOURCEUNLOADEDREQUEST_H