#ifndef EGE_CORE_RESOURCE_MULTITHREAD_RESOURCEMANAGERREQUEST_H
#define EGE_CORE_RESOURCE_MULTITHREAD_RESOURCEMANAGERREQUEST_H

/*! Multithreaded resource manager's request base type. This is a base for all kinds of requests which needs can originate from working thread,
 *  need to be stored and propagated further from main thread.
 */

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Available request types. */
enum RequestType
{
  EGroupLoaded = 0,          /*!< Group has been loaded request. */
  EGroupUnloaded            /*!< Group has been unloaded request. */
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceManagerRequest
{
  public:

    virtual ~ResourceManagerRequest() {}

    EGE_DEFINE_NEW_OPERATORS_INLINE
    EGE_DEFINE_DELETE_OPERATORS_INLINE

    /*! Returns request type. */
    virtual RequestType type() const = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCE_MULTITHREAD_RESOURCEMANAGERREQUEST_H