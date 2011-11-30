#ifndef EGE_CORE_LIST_H
#define EGE_CORE_LIST_H

#include "Core/Platform.h"
#include <list>
#include <algorithm>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class List : public std::list<T>
{
  public:

    List();
    List(const T& object);

    /* Removes object at given index. */
    void removeAt(s32 index);
    /* Returns last element. If list is empty, default value is returned. */
    const T last(const T& defaultValue) const;
    /* Returns TRUE if given object is present. */
    inline bool contains(const T object) const;
    /* Copies all data from given list. */
    void copy(const List& other);
    /* Appends given list. */
    List& operator << (const List& other);
    /* Appends given element. */
    List& operator << (const T& value);
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
List<T>::List()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
List<T>::List(const T& object)
{
  push_back(object);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes object at given index. */
template <typename T>
void List<T>::removeAt(s32 index)
{
  if (index < static_cast<s32>(this->size()))
  {
    typename List<T>::iterator it = this->begin();
    while (0 != index--)
    {
      ++it;
    }

    erase(it);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns last element. If list is empty, default value is returned. */
template <typename T>
const T List<T>::last(const T& defaultValue) const
{
  return (0 == this->size()) ? defaultValue : this->at(this->size() - 1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if given object is present. */
template <typename T>
bool List<T>::contains(const T object) const
{
  typename List<T>::const_iterator it = std::find(this->begin(), this->end(), object);
  return (it != this->end());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Copies all data from given list. */
template <typename T>
void List<T>::copy(const List<T>& other)
{
  this->clear();
  this->insert(this->begin(), other.begin(), other.end());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Appends given list. */
template <typename T>
List<T>& List<T>::operator << (const List<T>& other)
{
  this->insert(this->end(), other.begin(), other.end());
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Appends given element. */
template <typename T>
List<T>& List<T>::operator << (const T& value)
{
  this->push_back(value);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_LIST_H