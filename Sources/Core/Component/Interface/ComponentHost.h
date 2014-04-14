#ifndef EGE_CORE_COMPONENTS_COMPONENTHOST_H
#define EGE_CORE_COMPONENTS_COMPONENTHOST_H

/*! Component host is container for another components.
 *  @note This class in not thread safe.
 */

#include "EGE.h"
#include "EGEString.h"
#include "EGEMap.h"
#include "EGEList.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Component, PComponent)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ComponentHost
{
  public:

    ComponentHost();
    virtual ~ComponentHost();

    /*! Adds given component to pool.
     *  @param  Component Component to be added.
     *  @return EGE_SUCCESS if component has been added. 
     *          EGE_ERROR_ALREADY_EXISTS if component with given name and type is already in the pool.
     *  @note Derived classes may change the behavior ie put the restriction on number of instances of the given type or refuse to add certain types of 
     *        components.
     */
    virtual EGEResult addComponent(const PComponent& component);
    /*! Returns the list of components of a given type. 
     *  @param  type  Component type to return.
     *  @return List of all attached components of the given type.
     */
    List<PComponent> components(u32 type) const;
    /*! Returns first component of the given type. 
     *  @param  type  Component type to return.
     *  @return First component of given type found.
     *  @note This is convinience method.
     */
    PComponent component(u32 type) const;
    /*! Returns component with a given name. 
     *  @param  name  Name of the component to return.
     *  @return First component of with a given name found.
     *  @note This is convinience method.
     */
    PComponent component(const String& name) const;

    /*! Removes given component. 
     *  @param  component Component to remove.
     */
    void removeComponent(const PComponent& component);

  protected:

    typedef MultiMap<u32, PComponent> ComponentPool;

  protected:

    /*! Pool of attached components, refernced by unique object id. */
    ComponentPool m_pool;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_COMPONENTS_COMPONENTHOST_H