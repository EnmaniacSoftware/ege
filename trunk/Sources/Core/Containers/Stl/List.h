#ifndef EGE_CORE_LIST_H
#define EGE_CORE_LIST_H

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "Core/Platform.h"
#include <list>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class EGEList : public std::list<T>
{
  public:

    EGEList();
    EGEList(const T& object);

    /* Returns last element. If list is empty, default value is returned. */
    const T last(const T& defaultValue) const;
    /* Returns TRUE if given object is present. */
    inline bool contains(const T object) const;
    /* Copies all data from given list. */
    void copy(const EGEList& other);
    /* Appends given list. */
    EGEList& operator << (const EGEList& other);
    /* Appends given element. */
    EGEList& operator << (const T& value);
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
EGEList<T>::EGEList()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
EGEList<T>::EGEList(const T& object)
{
  push_back(object);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns last element. If list is empty, default value is returned. */
template <typename T>
const T EGEList<T>::last(const T& defaultValue) const
{
  return (0 == this->size()) ? defaultValue : this->at(this->size() - 1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if given object is present. */
template <typename T>
bool EGEList<T>::contains(const T object) const
{
  typename EGEList<T>::const_iterator it = std::find(this->begin(), this->end(), object);
  return (it != this->end());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Copies all data from given list. */
template <typename T>
void EGEList<T>::copy(const EGEList<T>& other)
{
  this->clear();
  this->insert(this->begin(), other.begin(), other.end());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Appends given list. */
template <typename T>
EGEList<T>& EGEList<T>::operator << (const EGEList<T>& other)
{
  this->insert(this->end(), other.begin(), other.end());
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Appends given element. */
template <typename T>
EGEList<T>& EGEList<T>::operator << (const T& value)
{
  this->push_back(value);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_LIST_H