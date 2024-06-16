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

    Factory(Engine& engine) : m_engine(engine) {}

  public:

    /*! Instance creating function. 
     *  @param  engine  Engine object.
     */
    typedef Type (*egeFactoryCreateFunction)(Engine& engine);

  public:

    /*! Registers object instance of a given type name. 
     *  @param  typeName       Type name of the interface to be registered. This needs to be unique.
     *  @param  createFunction Function capable of creating of new instance of the given interface.
     *  @return EGE_SUCCESS if instance has been successfully registered. Otherwise, another available error code.
     */
    virtual EGEResult registerInterface(const String& typeName, egeFactoryCreateFunction createFunction)
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
        m_registry.insert(typeName, createFunction);
      }

      return result;
    }

    /*! Creates instance of registered interface of the type given by name. 
     *  @param  typeName    Type name of the registered instance.
     *  @return Created instance of the given interface. NULL if error occured.
     */
    virtual Type createInstance(const String& typeName) const
    {
      Type object = NULL;

      // file entry with given type name
      const egeFactoryCreateFunction createFunction = m_registry.value(typeName, NULL);
      if (NULL != createFunction)
      {
        // create instance
        object = createFunction(m_engine);
      }

      return object;
    }

    /*! Returns whether given interface type is registered. 
     *  @param  typeName  Type name of the registered interface.
     *  @return TRUE if interface of a given type name has been already registered. Otherwise, FALSE.
     */
    virtual bool isInterfaceRegistered(const String& typeName) const
    {
      return m_registry.contains(typeName);
    }

  private:

    typedef Map<String, egeFactoryCreateFunction> Registry;

  private:
  
    /*! Engine. */
    Engine& m_engine;
    /*! Registry of registered instances. */
    Registry m_registry;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Type, typename P1>
class Factory1
{
  public:

    Factory1(Engine& engine) : m_engine(engine) {}

  public:

    /*! Instance creating function. 
     *  @param  engine  Engine object.
     *  @param  param1  Extra parameter.
     */
    typedef Type (*egeFactoryCreateFunction)(Engine& engine, P1 param1);

  public:

    /*! Registers object instance of a given type name. 
     *  @param  typeName       Type name of the interface to be registered. This needs to be unique.
     *  @param  createFunction Function capable of creating of new instance of the given interface.
     *  @return EGE_SUCCESS if instance has been successfully registered. Otherwise, another available error code.
     */
    virtual EGEResult registerInterface(const String& typeName, egeFactoryCreateFunction createFunction)
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
        m_registry.insert(typeName, createFunction);
      }

      return result;    
    }

    /*! Creates instance of registered interface of the type given by name. 
     *  @param  typeName    Type name of the registered instance.
     *  @param  param1      Extra parameter to be passed to created instance.
     *  @return Created instance of the given interface. NULL if error occured.
     */
    virtual Type createInstance(const String& typeName, P1 param1) const
    {
      Type object = NULL;

      // file entry with given type name
      const egeFactoryCreateFunction createFunction = m_registry.value(typeName, NULL);
      if (NULL != createFunction)
      {
        // create instance
        object = createFunction(m_engine, param1);
      }

      return object;    
    }

    /*! Returns whether given interface type is registered. 
     *  @param  typeName  Type name of the registered interface.
     *  @return TRUE if interface of a given type name has been already registered. Otherwise, FALSE.
     */
    virtual bool isInterfaceRegistered(const String& typeName) const
    {
      return m_registry.contains(typeName);
    }

  private:

    typedef Map<String, egeFactoryCreateFunction> Registry;

  private:
  
    /*! Engine. */
    Engine& m_engine;
    /*! Registry of registered instances. */
    Registry m_registry;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_FACTORY_FACTORY_H
