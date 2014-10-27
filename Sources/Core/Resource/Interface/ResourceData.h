#ifndef EGE_CORE_RESOURCEDATA_H
#define EGE_CORE_RESOURCEDATA_H

/** Data resource definition class. This object (resource) contains definition of location to particular data buffer (file).
 *   Upon loading it contains data buffer filled with data in the given location.
 */

#include "EGE.h"
#include "Core/Data/Interface/DataBuffer.h"
#include "Core/Resource/Interface/Resource.h"
#include "EGEXml.h"
#include "EGEString.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceGroup;

EGE_DECLARE_SMART_CLASS(ResourceData, PResourceData)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceData : public IResource
{
  public:

    ResourceData(Engine& engine, ResourceGroup* group);
    virtual ~ResourceData();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! @see IResource::name. */
    const String& name() const override;
    /*! @see IResource::create. */ 
    EGEResult create(const String& path, const PObject& data) override;
    /*! @see IResource::load. */
    EGEResult load() override;
    /*! @see IResource::unload. */
    void unload() override;

    /*! Gets instance of data object defined by resource. */
    PDataBuffer data() const { return m_data; }

  private:

    /*! Returns whether data is NULL terminated.*/
    bool isNulled() const { return m_nulled; }

  private:

    /*! Name. */
    String m_name;
    /*! Flag indicating if data should be NULL terminated upon loading. */
    bool m_nulled;
    /*! Data object created from resource. NULL if not created yet. */
    PDataBuffer m_data;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCEDATA_H
