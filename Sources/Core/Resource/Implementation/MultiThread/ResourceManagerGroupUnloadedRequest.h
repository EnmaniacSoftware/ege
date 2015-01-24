#ifndef EGE_CORE_RESOURCE_MULTITHREAD_RESOURCEMANAGERGROUPUNLOADEDREQUEST_H
#define EGE_CORE_RESOURCE_MULTITHREAD_RESOURCEMANAGERGROUPUNLOADEDREQUEST_H

/*! Multithreaded resource manager's group unloaded request type. 
 *  This request represent completion of resource group unloading.
 */

#include "Core/Resource/Implementation/MultiThread/ResourceManagerRequest.h"

EGE_NAMESPACE_BEGIN

  //--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceManagerGroupUnloadedRequest : public ResourceManagerRequest
{
  public:

    ResourceManagerGroupUnloadedRequest(const PResourceGroup& group, EGEResult result) 
    : m_group(group)
    , m_result(result) {}

    /*! Returns group. */
    const PResourceGroup& group() const { return m_group; }
    /*! Returns request result. */
    EGEResult result() const { return m_result; }

  private:

    /*! @see ResourceManagerRequest::type. */
    RequestType type() const override { return EGroupUnloaded; }

  private:

    /*! Group. */
    PResourceGroup m_group;
    /*! Result. */
    EGEResult m_result;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCE_MULTITHREAD_RESOURCEMANAGERGROUPUNLOADEDREQUEST_H