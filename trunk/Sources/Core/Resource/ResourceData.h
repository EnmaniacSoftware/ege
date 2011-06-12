#ifndef EGE_CORE_RESOURCEDATA_H
#define EGE_CORE_RESOURCEDATA_H

#include "EGE.h"
#include "Core/Data/DataBuffer.h"
#include "EGEXml.h"
#include "EGEString.h"
#include "Core/Resource/Resource.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceManager;

EGE_DECLARE_SMART_CLASS(ResourceData, PResourceData)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceData : public IResource
{
  public:

    virtual ~ResourceData();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Creates instance of resource. This method is a registration method for manager. */
    static PResource Create(Application* app, ResourceManager* manager);

    /* IResource override. Returns name of resource. */
    const EGEString& name() const override;
    /* Initializes resource from XML. 
    * 
    *  \param  path  full path to resource definition file.
    *  \param  tag   xml element with resource definition. 
    */
    EGEResult create(const EGEString& path, const PXmlElement& tag) override;
    /* IResource override. Loads resource. */
    EGEResult load() override;
    /* IResource override. Unloads resource. */
    void unload() override;
    /*! Gets instance of data object defined by resource. */
    inline PDataBuffer data() const { return m_data; }

  private:

    ResourceData(Application* app, ResourceManager* manager);
    /*! Returns TRUE if material is loaded. */
    inline bool isLoaded() const { return NULL != m_data; }
    /*! Gets path to texture file. */
    inline const EGEString& path() const { return m_path; } 
    /*! Returns whether data is NULL terminated.*/
    inline bool isNulled() const { return m_nulled; } 

  private:

    /*! Name. */
    EGEString m_name;
    /*! Full path to data file. */
    EGEString m_path;
    /*! Flag indicating if data should be NULL terminated upon loading. */
    bool m_nulled;
    /*! Data object created from resource. NULL if not created yet. */
    PDataBuffer m_data;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCEDATA_H
