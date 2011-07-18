#ifndef EGE_CORE_SMARTPOINTER_H
#define EGE_CORE_SMARTPOINTER_H

/** Class implements smart pointer by means of reference counter. It is intended to work with Object-dervied objects only.
 */

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "Core/Platform.h"
#include "Core/Memory/Object.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Object;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class SmartPointer
{
  public:

    SmartPointer(T* object = NULL);
    /* Constructor for wrapping stack allocated objects into their P-class representation. This conversion does not allow wrapped object to be deallocated. */
    SmartPointer(T& object);
    SmartPointer(const SmartPointer<T>& other);
    template <typename U>
    SmartPointer(const SmartPointer<U>& other);

   ~SmartPointer();

    T& operator=(T* object);
    T& operator=(const SmartPointer<T>& other);

    template <typename U>
    T& operator=(const SmartPointer<U>& other);

    T* operator->() const;
    T* object() const;
    T& objectRef() const;

    inline operator T*() const { return m_object; }

    /*! Returns TRUE if smart pointer points to any object. */
    inline bool isValid() const { return NULL != m_object; }

    T* m_object;

  private:

    inline void incrementReference(Object* object)
    {
      if (object)
      {
        object->addReference();
      }
    }

    inline void decrementReference(Object* object)
    {
      if (object)
      {
        object->release();
      }
    }
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
SmartPointer<T>::SmartPointer(T* object)
{
  m_object = object;

  // NOTE: This constructor is used while wrapping non P-class object into its P-class version. Cause usually we want to wrap objects allocated on stack which
  //       later we dont want to deallocate, we dont touch the ref counter so it does not gets deallocated.
  // NOTE: In case later it will trun out this is also called for some other scenario, this needs to be given another thought so the case above is fulfield as
  //       well.
  incrementReference(object);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Constructor for wrapping stack allocated objects into their P-class representation. This conversion does not allow wrapped object to be deallocated. */
template <typename T>
SmartPointer<T>::SmartPointer(T& object)
{
  m_object = &object;

  // NOTE: do not increment ref counter
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
SmartPointer<T>::SmartPointer(const SmartPointer<T>& other)
{
  m_object = other.object();
  incrementReference(m_object);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
template <typename U>
SmartPointer<T>::SmartPointer(const SmartPointer<U>& other)
{
  m_object = static_cast<T*>(other.object());
  incrementReference(m_object);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
SmartPointer<T>::~SmartPointer()
{
  decrementReference(static_cast<Object*>(m_object));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T& SmartPointer<T>::operator=(T* object)
{
  decrementReference(m_object);
  m_object = object;
  incrementReference(m_object);
  return *m_object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T& SmartPointer<T>::operator=(const SmartPointer<T>& other)
{
  decrementReference(m_object);
  m_object = other.object();
  incrementReference(m_object);
  return *m_object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
template <typename U>
T& SmartPointer<T>::operator=(const SmartPointer<U>& other)
{
  decrementReference(m_object);
  m_object = (T*)(other.object());
  incrementReference(m_object);
  return *m_object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T* SmartPointer<T>::operator->() const
{
  return m_object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T* SmartPointer<T>::object() const
{
  return m_object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T& SmartPointer<T>::objectRef() const
{
  return *m_object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SMARTPOINTER_H