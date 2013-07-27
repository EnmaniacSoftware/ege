#ifndef EGE_CORE_SMARTPOINTER_H
#define EGE_CORE_SMARTPOINTER_H

/** Class implements smart pointer by means of reference counter. It is intended to work with Object-dervied objects only.
 */

#include "Core/Platform.h"
#include "Core/Memory/Object.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Object;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
class SmartPointer
{
  public:

    SmartPointer(T* object = NULL);
    /*! Constructor for wrapping stack allocated objects into their P-class representation. This conversion does not allow wrapped object to be deallocated. */
    SmartPointer(T& object);
    SmartPointer(const SmartPointer<T>& other);
    template <class U>
    SmartPointer(const SmartPointer<U>& other);

   ~SmartPointer();

    T& operator=(T* object);
    T& operator=(const SmartPointer<T>& other);

    template <class U>
    T& operator=(const SmartPointer<U>& other);

    T* operator->() const;
    T* object() const;

    operator T*() const { return m_object; }

    /*! Returns TRUE if smart pointer points to any object. */
    bool isValid() const { return NULL != m_object; }
    /*! Returns TRUE if object is deallocable. */
    bool isDeallocable() const { return m_deallocable; }

  public:

    /*! Object wrapped around smart pointer. */
    T* m_object;

  private:

    void incrementReference(Object* object);
    void decrementReference(Object* object);

  private:

    /*! TRUE if object should be deallocated. */
    bool m_deallocable;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
SmartPointer<T>::SmartPointer(T* object) : m_object(object), 
                                           m_deallocable(true)
{
  incrementReference(object);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
SmartPointer<T>::SmartPointer(T& object) : m_object(&object), 
                                           m_deallocable(false)
{
  // NOTE: do not increment ref counter
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
SmartPointer<T>::SmartPointer(const SmartPointer<T>& other) : m_object(other.object()), 
                                                              m_deallocable(other.m_deallocable)
{
  incrementReference(m_object);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
template <class U>
SmartPointer<T>::SmartPointer(const SmartPointer<U>& other) : m_object(static_cast<T*>(other.object())), 
                                                              m_deallocable(other.isDeallocable())
{
  incrementReference(m_object);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
SmartPointer<T>::~SmartPointer()
{
  decrementReference(static_cast<Object*>(m_object));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
inline T& SmartPointer<T>::operator = (T* object)
{
  // NOTE: use it for heap allocated objects only
  decrementReference(m_object);
  m_object = object;
  m_deallocable = true;
  incrementReference(m_object);
  return *m_object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
inline T& SmartPointer<T>::operator = (const SmartPointer<T>& other)
{
  decrementReference(m_object);
  m_object = other.object();
  m_deallocable = other.m_deallocable;
  incrementReference(m_object);
  return *m_object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
template <class U>
inline T& SmartPointer<T>::operator = (const SmartPointer<U>& other)
{
  decrementReference(m_object);
  m_object = (T*)(other.object());
  m_deallocable = other.isDeallocable();
  incrementReference(m_object);
  return *m_object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
inline T* SmartPointer<T>::operator -> () const
{
  return m_object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
inline T* SmartPointer<T>::object() const
{
  return m_object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
inline void SmartPointer<T>::incrementReference(Object* object)
{
  if ((NULL != object) && m_deallocable)
  {
    object->addReference();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
inline void SmartPointer<T>::decrementReference(Object* object)
{
  if ((NULL != object) && m_deallocable)
  {
    object->release();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SMARTPOINTER_H