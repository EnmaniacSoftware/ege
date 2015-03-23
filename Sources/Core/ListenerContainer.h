#ifndef EGE_CORE_LISTENERCONTAINER_H
#define EGE_CORE_LISTENERCONTAINER_H

#include "EGESet.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template<typename T>
class ListenerContainer
{
  protected:

    typedef Set<T*> ListenersContainer;
   
  public:

    ListenerContainer();
    virtual ~ListenerContainer();

    /*! Adds object to listeners pool. 
     *  @param  object  Object to be added into the pool.
     *  @return TRUE if object has been added into the pool. Otherwise, FALSE.
     */
    virtual bool addListener(T* const object);

    /*! Checks if given object is in the listsners pool.
     *  @param  object  Object to check against the pool.
     *  @return TRUE if object is in the pool. Otherwise, FALSE.
     */
    bool isListening(T* const object) const;
    /*! Removes object from listeners pool. 
     *  @param  object  Object to be removed from pool.
     */

    void removeListener(T* const object);
    /*! Removes all objects from listeners pool. */
    void removeAllListeners();
    
    /*! Returns pool of listeners. */
    const ListenersContainer& listeners() const;

  private:

    /*! Pool of listeners. */
    ListenersContainer m_listeners;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template<typename T>
ListenerContainer<T>::ListenerContainer()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template<typename T>
ListenerContainer<T>::~ListenerContainer()
{
  removeAllListeners();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template<typename T>
bool ListenerContainer<T>::addListener(T* const object)
{
  if (NULL == object)
  {
    return false;
  }

  // add into pool
  m_listeners.insert(object);
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template<typename T>
void ListenerContainer<T>::removeListener(T* const object)
{
  m_listeners.remove(object);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template<typename T>
void ListenerContainer<T>::removeAllListeners()
{
  m_listeners.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template<typename T>
bool ListenerContainer<T>::isListening(T* const object) const
{
  return m_listeners.contains(object);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template<typename T>
const typename ListenerContainer<T>::ListenersContainer& ListenerContainer<T>::listeners() const 
{ 
  return m_listeners; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_LISTENERCONTAINER_H