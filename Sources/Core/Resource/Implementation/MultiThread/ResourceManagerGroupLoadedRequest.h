#ifndef EGE_CORE_RESOURCE_MULTITHREAD_RESOURCEMANAGERGROUPLOADEDREQUEST_H
#define EGE_CORE_RESOURCE_MULTITHREAD_RESOURCEMANAGERGROUPLOADEDREQUEST_H

/*! Multithreaded resource manager's group loaded request type. 
 *  This request represent completion of resource group loading.
 */

#include "EGE.h"
#include "Core/Resource/Implementation/MultiThread/ResourceManagerRequest.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_PREDECLARE_SMART_CLASS(ResourceGroup, PResourceGroup)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceManagerGroupLoadedRequest : public ResourceManagerRequest
{
  public:

    ResourceManagerGroupLoadedRequest(const PResourceGroup& group, EGEResult result) 
    : m_group(group)
    , m_result(result) {}

    /*! Returns group. */
    const PResourceGroup& group() const { return m_group; }
    /*! Returns request result. */
    EGEResult result() const { return m_result; }

  private:

    /*! @see ResourceManagerRequest::type. */
    RequestType type() const override { return EGroupLoaded; }

  private:

    /*! Group. */
    PResourceGroup m_group;
    /*! Result. */
    EGEResult m_result;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCE_MULTITHREAD_RESOURCEMANAGERGROUPLOADEDREQUEST_H