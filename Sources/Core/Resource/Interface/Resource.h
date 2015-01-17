#ifndef EGE_CORE_RESOURCE_H
#define EGE_CORE_RESOURCE_H

/** Abstract resource definition class. This object is a base for all resource objects which can be registered in EGE Framework. Registered resources are 
    created while found in resource groups. When found framework will call proper object methods ie. to let the resource parse itself from group data, load,
    unload etc.
 */

#include "EGE.h"
#include "EGEString.h"
#include "Core/Memory/Object.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceGroup;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(IResource, PResource)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IResource : public Object
{
  /*! For overriding group while overriding resources. */
  friend class ResourceGroup;

  public:

    /*! Available states. */
    enum State
    {
      STATE_INVALID = 0,    /*!< Resource is not initialized. */
      STATE_UNLOADED,       /*!< Resource is initialized but not loaded yet. */
      STATE_LOADING,        /*!< Resource is being loaded. */
      STATE_LOADED          /*!< Resource is loaded and ready to use. */
    };

  public:

    IResource(Engine& engine, ResourceGroup* group, const String& typeName, egeObjectDeleteFunc deleteFunc = NULL);
    virtual ~IResource();

    /*! Initializes resource. 
     *  @param  path  Full path to resource definition file.
     *  @param  data  Source of resource data. 
     *  @return EGE_SUCCESS on success. EGE_ERROR_NOT_SUPPORTED if the source is not supported. Otherwise, one of the other error codes.
     */
    virtual EGEResult create(const String& path, const PObject& data) = 0;
    /*! Loads resource. 
     *  @return Returns EGE_SUCCESS if resource has been loaded. EGE_WAIT if resource is being loaded.
     */
    virtual EGEResult load() = 0;
    /*! Unloads resource. */
    virtual void unload() = 0;
    /*! Returns name of resource. */
    virtual const String& name() const = 0;

    /*! Returns current state. */
    State state() const;
    /*! Returns resource type name. */
    const String& typeName() const;
    /*! Returns TRUE if resource is manual. */
    bool isManual() const;
    /*! Returns path to resource. */
    const String& path() const;

  protected:

    /*! Returns pointer to owning group. */
    ResourceGroup* group() const;
    /*! Returns engine object. */
    Engine& engine() const;

  protected:

    /*! Manual flag. Manual resources wont be automatically handled by framework (ie. loaded/unloaded). */
    bool m_manual;
    /*! Path to resource. Can be empty if not required. */
    String m_path;
    /*! Current state. */
    volatile State m_state;

  private:

    /*! Reference to engine. */
    Engine& m_engine;
    /*! Type name. */
    String m_typeName;
    /*! Pointer to owning resource group. */
    ResourceGroup* m_group;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCE_H