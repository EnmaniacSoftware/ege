#ifndef EGE_CORE_RESOURCEDATA_H
#define EGE_CORE_RESOURCEDATA_H

/** Data resource definition class. This object (resource) contains definition of location to particular data buffer (file).
 *   Upon loading it contains data buffer filled with data in the given location.
 */

#include <EGE.h>
#include "Core/Data/DataBuffer.h"
#include "Core/Resource/Resource.h"
#include <EGEXml.h>
#include <EGEString.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceGroup;

EGE_DECLARE_SMART_CLASS(ResourceData, PResourceData)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceData : public IResource
{
  public:

    virtual ~ResourceData();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Creates instance of resource. This method is a registration method for manager. */
    static PResource Create(Application* app, ResourceGroup* group);

    /* IResource override. Returns name of resource. */
    const String& name() const override;
    /* Initializes resource from XML. 
    * 
    *  \param  path  full path to resource definition file.
    *  \param  tag   xml element with resource definition. 
    */
    EGEResult create(const String& path, const PXmlElement& tag) override;
    /* IResource override. Loads resource. */
    EGEResult load() override;
    /* IResource override. Unloads resource. */
    void unload() override;
    /*! Gets instance of data object defined by resource. */
    PDataBuffer data() const { return m_data; }

  private:

    ResourceData(Application* app, ResourceGroup* group);
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
