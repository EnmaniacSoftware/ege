#ifndef EGE_CORE_FACTORY_FACTORY_H
#define EGE_CORE_FACTORY_FACTORY_H

/*! Templated factory (utility) class. It allows registration of specific object which later on can be created by factory itself.
 */

#include "EGEMap.h"
#include "EGEStringList.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Type, typename CreateFuncDeclaration>
class Factory
{
  public:

    Factory(Engine& engine);
   ~Factory();

  public:

    /*! Registers object instance of a given type name. 
     *  @param  typeName    Type name of the interface to be registered. This needs to be unique.
     *  @param  createFunc  Function returning new instance of the given interface.
     *  @return EGE_SUCCESS if instance has been successfully registered. Otherwise, another available error code.     
     */
    EGEResult registerInterface(const String& typeName, CreateFuncDeclaration createFunc);
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

    typedef Map<String, CreateFuncDeclaration> Registry;

  private:
  
    /*! Engine. */
    Engine& m_engine;
    /*! Registry of registered instances. */
    Registry m_registry;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Type, typename CreateFuncDeclaration>
Factory<Type, CreateFuncDeclaration>::Factory(Engine& engine)
: m_engine(engine)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Type, typename CreateFuncDeclaration>
Factory<Type, CreateFuncDeclaration>::~Factory()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Type, typename CreateFuncDeclaration>
EGEResult Factory<Type, CreateFuncDeclaration>::registerInterface(const String& typeName, CreateFuncDeclaration createFunc)
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
    m_registry.insert(typeName, createFunc);
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Type, typename CreateFuncDeclaration>
Type Factory<Type, CreateFuncDeclaration>::createInstance(const String& typeName) const
{
  Type object = NULL;

  // file entry with given type name
  CreateFuncDeclaration createFunc = m_registry.value(typeName, NULL);
  if (NULL != createFunc)
  {
    // create instance
    object = createFunc(engine());
  }

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Type, typename CreateFuncDeclaration>
bool Factory<Type, CreateFuncDeclaration>::isInterfaceRegistered(const String& typeName) const
{
  return m_registry.contains(typeName);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Type, typename CreateFuncDeclaration>
Engine& Factory<Type, CreateFuncDeclaration>::engine() const
{
  return m_engine;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Type, typename CreateFuncDeclaration>
StringList Factory<Type, CreateFuncDeclaration>::registeredInterfaces() const
{
  StringList list;

  for (Registry::const_iterator it = m_registry.begin(); it != m_registry.end(); ++it)
  {
    list << it->first;
  }

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_FACTORY_FACTORY_H
