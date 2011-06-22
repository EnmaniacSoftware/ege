#ifndef EGE_CORE_LISTENERCONTAINER_H
#define EGE_CORE_LISTENERCONTAINER_H

#include <EGE.h>
#include <EGEList.h>
//#include <EGEDebug.h>

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
    void removeListener(T* object);
    /* Removes all objects from listeners pool. */
    void removeAllListeners();
    /*! Returns pool of listeners. */
    inline const EGEList<T*>& listeners() const { return m_listeners; }

  private:

    /*! Pool lock flag. If set to TRUE all operations are deferred until pool is unlocked. */
    bool m_locked;
    /*! Pool of listeners. */
    EGEList<T*> m_listeners;
    /*! Pool of listeners to add once unlocked. */
    EGEList<T*> m_listenersToAdd;
    /*! Pool of listeners to remove once unlocked. */
    EGEList<T*> m_listenersToRemove;

  protected:

    /* Locks container. */
    void lockContainer();
    /* Unlocks container. */
    void unlockContainer();
    /* Returns TRUE if container is locked. */
    inline bool isContainerLocked() const;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template<typename T>
ListenerContainer<T>::ListenerContainer()
{
  m_locked = false;
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

  if (isContainerLocked())
  {
    m_listenersToAdd.push_back(object);
    return true;
  }

  // add into pool
  m_listeners.push_back(object);
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes given pbject from listeners pool. */
template<typename T>
void ListenerContainer<T>::removeListener(T* object)
{
  if (NULL == object)
  {
    // do nothing
    return;
  }

  if (isContainerLocked())
  {
    m_listenersToRemove.push_back(object);
    return;
  }

  typename EGEList<T*>::iterator iter = std::find(m_listeners.begin(), m_listeners.end(), object);
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
  if (isContainerLocked())
  {
    m_listenersToRemove << m_listeners;
    return;
  }

  m_listeners.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if given object is in listeners pool. */
template<typename T>
bool ListenerContainer<T>::isListening(const T* object) const
{
  typename EGEList<T*>::const_iterator iter = std::find(m_listeners.begin(), m_listeners.end(), object);
  return (iter != m_listeners.end());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Locks container. */
template<typename T>
void ListenerContainer<T>::lockContainer()
{
  //EGE_ASSERT(m_locked);
  m_locked = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Unlocks container. */
template<typename T>
void ListenerContainer<T>::unlockContainer()
{
  //EGE_ASSERT(!m_locked);

  // remove pending
  for (typename EGEList<T*>::const_iterator iter = m_listenersToRemove.begin(); iter != m_listenersToRemove.end(); ++iter)
  {
    m_listeners.remove(*iter);
  }
  m_listenersToRemove.clear();

  // add pending
  m_listeners << m_listenersToAdd;
  m_listenersToAdd.clear();

  m_locked = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if container is locked. */
template<typename T>
bool ListenerContainer<T>::isContainerLocked() const
{
  return m_locked;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_LISTENERCONTAINER_H