#ifndef EGE_CORE_LISTENERCONTAINER_H
#define EGE_CORE_LISTENERCONTAINER_H

#include "EGETypes.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

template<typename T>
class ListenerContainer
{
  public:

    ListenerContainer();
    virtual ~ListenerContainer();

    /* Adds object to listeners pool. */
    virtual bool addListener(T* object);
    /* Returns TRUE if given object is in listeners pool. */
    bool isListening(const T* object) const;
    /* Removes given pbject from listeners pool. */
    void removeListener(const T* object);
    /* Removes all objects from listeners pool. */
    void removeAllListeners();
    /*! Returns pool of listeners. */
    inline const std::vector<T*>& listeners() const { return m_listeners; }

  private:

    /*! Pool of listeners. */
    std::vector<T*> m_listeners;
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
bool ListenerContainer<T>::addListener(T* object)
{
  if (NULL == object)
  {
    return false;
  }

  // add into pool
  m_listeners.push_back(object);
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

  typename std::vector<T*>::iterator iter = std::find(m_listeners.begin(), m_listeners.end(), object);
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
  typename std::vector<T*>::const_iterator iter = std::find(m_listeners.begin(), m_listeners.end(), object);
  return (iter != m_listeners.end());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_LISTENERCONTAINER_H