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

  operators:

    DynamicArray& operator << (const DynamicArray& other);
    DynamicArray& operator << (const T& value);

  public:

    /*! Removes element from this array. 
     *  @param  object  Element to be removed.
     */
    void remove(const T object);
    /*! Removes element at given index. 
     *  @param  index 0-based index of the element to be removed from this array.
     *  @note If index if out of range, no action is performed.
     */
    void removeAt(s32 index);

    /*! Returns last element. 
     *  @param  defaultValue  Default value to be returned in case of an empty array. 
     *  @return Last element or default value.
     */
    const T last(const T& defaultValue = T()) const;

    /*! Returns TRUE if the given element is present in this array. 
     *  @param  object  Element to be checked for existance.
     *  @return TRUE if given element is present. Otherwise, FALSE.
     */
    bool contains(const T object) const;

    /*! Copies all data from given array into current one. 
     *  @param  from  Array data is copied from.
     *  @note All old data in the current array is removed before the copy operation.
     */
    void copy(const DynamicArray& from);

    /*! Returns element at given position. 
     *  @param  index         0-based index of the element to be returned from this array.
     *  @param  defaultValue  Default value to be returned in case no element could be found. 
     *  @return Found element or default one.
     */
    const T& at(s32 index, const T& defaultValue) const;

    /*! Returns an index of first occurence of a given element. 
     *  @param  object  Object to test for occurence.
     *  @return 0-based index position of the given element within this array. Negative value is returned if it is not found.
     */
    s32 indexOf(const T& object) const;
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
  this->push_back(object);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void DynamicArray<T>::remove(const T object)
{
  typename DynamicArray<T>::iterator it = std::find(this->begin(), this->end(), object);
  if (it != this->end())
  {
    this->erase(it);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void DynamicArray<T>::removeAt(s32 index)
{
// TAGE - uncomment once EGE #110 is done
//  EGE_ASSERT(0 <= index);
//  EGE_ASSERT(index < size());

  if ((0 <= index) && (index < static_cast<s32>(this->size())))
  {
    typename DynamicArray<T>::iterator it = this->begin() + index;
    this->erase(it);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
const T DynamicArray<T>::last(const T& defaultValue) const
{
  return this->at(static_cast<s32>(this->size()) - 1, defaultValue);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool DynamicArray<T>::contains(const T object) const
{
  typename DynamicArray<T>::const_iterator it = std::find(this->begin(), this->end(), object);
  return (it != this->end());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void DynamicArray<T>::copy(const DynamicArray<T>& from)
{
  this->clear();
  this->insert(this->begin(), from.begin(), from.end());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
DynamicArray<T>& DynamicArray<T>::operator << (const DynamicArray<T>& other)
{
  this->insert(this->end(), other.begin(), other.end());
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
DynamicArray<T>& DynamicArray<T>::operator << (const T& value)
{
  this->push_back(value);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
const T& DynamicArray<T>::at(s32 index, const T& defaultValue) const
{
  if ((0 <= index) && (index < static_cast<s32>(this->size())))
  {
    return this->operator[](static_cast<size_t>(index));
  }

  return defaultValue;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
s32 DynamicArray<T>::indexOf(const T& object) const
{
  s32 i = 0;
  for (typename DynamicArray<T>::const_iterator it = this->begin(); it != this->end(); ++it, ++i)
  {
    if (*it == object)
    {
      // found
      return i;
    }
  }

  // not found
  return -1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CONTAINER_DYNAMICARRAY_H