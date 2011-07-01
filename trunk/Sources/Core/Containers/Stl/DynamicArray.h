#ifndef EGE_CORE_CONTAINER_DYNAMICARRAY_H
#define EGE_CORE_CONTAINER_DYNAMICARRAY_H

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "Core/Platform.h"
#include <vector>
#include <algorithm>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class EGEDynamicArray : public std::vector<T>
{
  public:

    EGEDynamicArray();
    EGEDynamicArray(const T& object);

    /* Removes given object from list. */
    void remove(const T object);
    /* Returns last element. If list is empty, default value is returned. */
    const T last(const T& defaultValue) const;
    /* Returns TRUE if given object is present. */
    inline bool contains(const T object) const;
    /* Copies all data from given list. */
    void copy(const EGEDynamicArray& other);
    /* Appends given list. */
    EGEDynamicArray& operator << (const EGEDynamicArray& other);
    /* Appends given element. */
    EGEDynamicArray& operator << (const T& value);
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
EGEDynamicArray<T>::EGEDynamicArray()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
EGEDynamicArray<T>::EGEDynamicArray(const T& object)
{
  push_back(object);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes given object from list. */
template <typename T>
void EGEDynamicArray<T>::remove(const T object)
{
  typename EGEDynamicArray<T>::iterator it = std::find(this->begin(), this->end(), object);
  if (it != this->end())
  {
    erase(it);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns last element. If list is empty, default value is returned. */
template <typename T>
const T EGEDynamicArray<T>::last(const T& defaultValue) const
{
  return (0 == this->size()) ? defaultValue : this->at(this->size() - 1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if given object is present. */
template <typename T>
bool EGEDynamicArray<T>::contains(const T object) const
{
  typename EGEDynamicArray<T>::const_iterator it = std::find(this->begin(), this->end(), object);
  return (it != this->end());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Copies all data from given list. */
template <typename T>
void EGEDynamicArray<T>::copy(const EGEDynamicArray<T>& other)
{
  this->clear();
  this->insert(this->begin(), other.begin(), other.end());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Appends given list. */
template <typename T>
EGEDynamicArray<T>& EGEDynamicArray<T>::operator << (const EGEDynamicArray<T>& other)
{
  this->insert(this->end(), other.begin(), other.end());
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Appends given element. */
template <typename T>
EGEDynamicArray<T>& EGEDynamicArray<T>::operator << (const T& value)
{
  this->push_back(value);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CONTAINER_DYNAMICARRAY_H