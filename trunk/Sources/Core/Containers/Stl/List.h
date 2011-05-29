#ifndef EGE_CORE_CONTAINER_LIST_H
#define EGE_CORE_CONTAINER_LIST_H

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class EGEList : public std::vector<T>
{
  public:

    /* Removes given object from list. */
    void remove(const T object);
    /* Returns last element. If list is empty, default value is returned. */
    const T last(const T& defaultValue) const;
    /* Returns TRUE if given object is present. */
    inline bool contains(const T& object) const;
    /* Copies all data from given list. */
    void copy(const EGEList& other);
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes given object from list. */
template <typename T>
void EGEList<T>::remove(const T object)
{
  typename EGEList<T>::iterator it = std::find(this->begin(), this->end(), object);
  if (it != this->end())
  {
    erase(it);
  }
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
bool EGEList<T>::contains(const T& object) const
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

EGE_NAMESPACE_END

#endif // EGE_CORE_CONTAINER_LIST_H