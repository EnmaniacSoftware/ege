#ifndef EGE_CORE_FACTORY_FACTORY_H
#define EGE_CORE_FACTORY_FACTORY_H

/*! Templated factory (utility) class. It allows registration of specific object which later on can be created by factory itself.
 */

#include "EGEMap.h"
#include "EGEStringList.h"
#include "EGEMemory.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Type>
class Factory
{
  public:

    Factory(Engine& engine);
   ~Factory();

  public:

    /*! Create instance functor interface. */
    class CreateInstanceFunctor : public Object
    {
      public:

        EGE_DEFINE_NEW_OPERATORS_INLINE
        EGE_DEFINE_DELETE_OPERATORS_INLINE

        /*! Creates instance of the given type. */
        virtual Type operator()() const = 0;
    };

    EGE_DECLARE_SMART_CLASS(CreateInstanceFunctor, PCreateInstanceFunctor)

  public:

    /*! Registers object instance of a given type name. 
     *  @param  typeName      Type name of the interface to be registered. This needs to be unique.
     *  @param  createFunctor Functor capable of creating of new instance of the given interface.
     *  @return EGE_SUCCESS if instance has been successfully registered. Otherwise, another available error code.
     */
    EGEResult registerInterface(const String& typeName, const PCreateInstanceFunctor& createFunctor);
    /*! Creates instance of registered interface of the type given by name. 
     *  @param  typeName    Type name of the registered instance.
     *  @return Created instance of the given interface. NULL if error occured.
     */
    Type createInstance(const String& typeName) const;
    /*! Returns whether given interface type is registered. 
     *  @param  typeName  Type name of the registered interface.
     *  @return TRUE if interface of a given type name has been already registered. Otherwise, FALSE.
     */
    bool isInterfaceRegistered(const String& typeName) const;
    /*! Returns names of registered interfaces. 
     *  @return List of names of registered interfaces.
     */
    StringList registeredInterfaces() const;

  private:
    
    /*! Returns engine reference. */
    Engine& engine() const;

  private:

    typedef Map<String, PCreateInstanceFunctor> Registry;

  private:
  
    /*! Engine. */
    Engine& m_engine;
    /*! Registry of registered instances. */
    Registry m_registry;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Type>
Factory<Type>::Factory(Engine& engine)
: m_engine(engine)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Type>
Factory<Type>::~Factory()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Type>
EGEResult Factory<Type>::registerInterface(const String& typeName, const PCreateInstanceFunctor& createFunctor)
{
  EGEResult result = EGE_SUCCESS;

  // check if interface with such a name exists already
  if (isInterfaceRegistered(typeName))
  {
    // error!
    result = EGE_ERROR_ALREADY_EXISTS;
  }
  else
  {
    // register
    m_registry.insert(typeName, createFunctor);
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Type>
Type Factory<Type>::createInstance(const String& typeName) const
{
  Type object = NULL;

  // file entry with given type name
  const PCreateInstanceFunctor& createFunctor = m_registry.value(typeName, NULL);
  if (NULL != createFunctor)
  {
    // create instance
    object = (*createFunctor)();
  }

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Type>
bool Factory<Type>::isInterfaceRegistered(const String& typeName) const
{
  return m_registry.contains(typeName);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Type>
Engine& Factory<Type>::engine() const
{
  return m_engine;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Type>
StringList Factory<Type>::registeredInterfaces() const
{
  StringList list;

  for (typename Registry::const_iterator it = m_registry.begin(); it != m_registry.end(); ++it)
  {
    list << it->first;
  }

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_FACTORY_FACTORY_H
