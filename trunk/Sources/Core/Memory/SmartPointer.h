#ifndef EGE_CORE_SMART_POINTER_H
#define EGE_CORE_SMART_POINTER_H

// NOTE: this class implements smart pointer by means of reference counter. It is intended to work with Object-dervied
//       objects only.

#include "EGE.h"
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
  incrementReference(object);
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
  decrementReference(m_object);
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

#endif // EGE_CORE_SMART_POINTER_H