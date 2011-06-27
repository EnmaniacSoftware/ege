#ifndef EGE_CORE_RESOURCE_H
#define EGE_CORE_RESOURCE_H

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

    IResource(Application* app, ResourceManager* manager, const EGEString& typeName, egeObjectDeleteFunc deleteFunc = NULL) 
    : Object(app, EGE_OBJECT_UID_RESOURCE, deleteFunc), m_manager(manager), m_typeName(typeName) {}
    virtual ~IResource() {}

    /* Initializes resource from XML. 
    * 
    *  \param  path  full path to resource definition file.
    *  \param  tag   xml element with resource definition. 
    */
    virtual EGEResult create(const EGEString& path, const PXmlElement& tag) = 0;
    /*! Loads resource. */
    virtual EGEResult load() = 0;
    /* Unloads resource. */
    virtual void unload() = 0;
    /* Returns name of resource. */
    virtual const EGEString& name() const = 0;
    /*! Returns resource type name. */
    inline const EGEString& typeName() const { return m_typeName; }

  protected:

    /*! Gets owning manager. */
    inline ResourceManager* manager() const { return m_manager; }

  private:

    /*! Pointer to owning manager */
    ResourceManager* m_manager;
    /*! Type name. */
    EGEString m_typeName;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCE_H