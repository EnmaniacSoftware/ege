#ifndef EGE_CORE_CONTAINER_DYNAMICARRAY_H
#define EGE_CORE_CONTAINER_DYNAMICARRAY_H

#include "Core/Platform.h"
#include <vector>
#include <algorithm>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class DynamicArray : public std::vector<T>
{
  public:

    DynamicArray();
    DynamicArray(const T& object);

    /* Removes given object from list. */
    void remove(const T object);
    /* Removes object at given index. */
    void removeAt(s32 index);
    /* Returns last element. If list is empty, default value is returned. */
    const T last(const T& defaultValue) const;
    /* Returns TRUE if given object is present. */
    inline bool contains(const T object) const;
    /* Copies all data from given list. */
    void copy(const DynamicArray& other);
    /* Appends given list. */
    DynamicArray& operator << (const DynamicArray& other);
    /* Appends given element. */
    DynamicArray& operator << (const T& value);
    /* Returns object at given position. If not found, returns default object. */
    const T at(s32 index, const T& defaultValue) const;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
DynamicArray<T>::DynamicArray()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
DynamicArray<T>::DynamicArray(const T& object)
{
  push_back(object);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes given object from list. */
template <typename T>
void DynamicArray<T>::remove(const T object)
{
  typename DynamicArray<T>::iterator it = std::find(this->begin(), this->end(), object);
  if (it != this->end())
  {
    erase(it);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes object at given index. */
template <typename T>
void DynamicArray<T>::removeAt(s32 index)
{
  if (index < static_cast<s32>(this->size()))
  {
    typename DynamicArray<T>::iterator it = this->begin() + index;
    erase(it);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns last element. If list is empty, default value is returned. */
template <typename T>
const T DynamicArray<T>::last(const T& defaultValue) const
{
  return (0 == this->size()) ? defaultValue : this->at(this->size() - 1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if given object is present. */
template <typename T>
bool DynamicArray<T>::contains(const T object) const
{
  typename DynamicArray<T>::const_iterator it = std::find(this->begin(), this->end(), object);
  return (it != this->end());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Copies all data from given list. */
template <typename T>
void DynamicArray<T>::copy(const DynamicArray<T>& other)
{
  this->clear();
  this->insert(this->begin(), other.begin(), other.end());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Appends given list. */
template <typename T>
DynamicArray<T>& DynamicArray<T>::operator << (const DynamicArray<T>& other)
{
  this->insert(this->end(), other.begin(), other.end());
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Appends given element. */
template <typename T>
DynamicArray<T>& DynamicArray<T>::operator << (const T& value)
{
  this->push_back(value);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns object at given position. If not found, returns default object. */
template <typename T>
const T DynamicArray<T>::at(s32 index, const T& defaultValue) const
{
  return ((0 <= index) && (index < static_cast<s32>(this->size()))) ? this->operator[](static_cast<size_t>(index)) : defaultValue;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CONTAINER_DYNAMICARRAY_H