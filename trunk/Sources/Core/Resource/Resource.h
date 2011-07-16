#ifndef EGE_CORE_RESOURCE_H
#define EGE_CORE_RESOURCE_H

/** Abstract resource definition class. This object is a base for all resource objects which can be registered in EGE Framework. Registered resources are 
    created while found in resource groups. When found framework will call proper object methods ie. to let the resource parse itself from group data, load,
    unload etc.
 */

#include <EGE.h>
#include <EGEXml.h>
#include "Core/Memory/Object.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceManager;
class Application;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(IResource, PResource)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class IResource : public Object
{
  public:

    IResource(Application* app, ResourceManager* manager, const String& typeName, egeObjectDeleteFunc deleteFunc = NULL) 
    : Object(app, EGE_OBJECT_UID_RESOURCE, deleteFunc), m_manager(manager), m_typeName(typeName) {}
    virtual ~IResource() {}

    /* Initializes resource from XML. 
    * 
    *  \param  path  full path to resource definition file.
    *  \param  tag   xml element with resource definition. 
    */
    virtual EGEResult create(const String& path, const PXmlElement& tag) = 0;
    /*! Loads resource. */
    virtual EGEResult load() = 0;
    /* Unloads resource. */
    virtual void unload() = 0;
    /* Returns name of resource. */
    virtual const String& name() const = 0;
    /*! Returns resource type name. */
    inline const String& typeName() const { return m_typeName; }

  protected:

    /*! Gets owning manager. */
    inline ResourceManager* manager() const { return m_manager; }

  private:

    /*! Pointer to owning manager */
    ResourceManager* m_manager;
    /*! Type name. */
    String m_typeName;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCE_H