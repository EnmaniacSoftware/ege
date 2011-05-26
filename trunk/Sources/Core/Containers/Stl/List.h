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

EGE_NAMESPACE_END

#endif // EGE_CORE_CONTAINER_LIST_H