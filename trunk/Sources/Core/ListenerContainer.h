#ifndef EGE_CORE_LISTENERCONTAINER_H
#define EGE_CORE_LISTENERCONTAINER_H

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include <EGETree.h>
//#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

template<typename T>
class ListenerContainer
{
  protected:

    typedef EGETree<T*> ListenersContainer;
    //typedef typename EGETree<T*> ListenersContainerIterator;
   
  public:

    ListenerContainer();
    virtual ~ListenerContainer();

    /* Adds object to listeners pool. */
    virtual bool addListener(T* const object);
    /* Returns TRUE if given object is in listeners pool. */
    bool isListening(const T* object) const;
    /* Removes given pbject from listeners pool. */
    void removeListener(const T* object);
    /* Removes all objects from listeners pool. */
    void removeAllListeners();
    /*! Returns pool of listeners. */
    inline const ListenersContainer& listeners() const { return m_listeners; }

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
/*! Adds object to listeners pool. */
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
/*! Removes given pbject from listeners pool. */
template<typename T>
void ListenerContainer<T>::removeListener(const T* object)
{
  if (NULL == object)
  {
    // do nothing
    return;
  }

  typename EGETree<T*>::const_iterator iter = m_listeners.find(const_cast<T*>(object));
  if (iter != m_listeners.end())
  {
    m_listeners.erase(iter);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes all objects from listeners pool. */
template<typename T>
void ListenerContainer<T>::removeAllListeners()
{
  m_listeners.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if given object is in listeners pool. */
template<typename T>
bool ListenerContainer<T>::isListening(const T* object) const
{
  typename EGETree<T*>::const_iterator iter = m_listeners.find(const_cast<T*>(object));
  return (iter != m_listeners.end());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_LISTENERCONTAINER_H