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
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes given object from list. */
template <typename T>
void EGEList<T>::remove(const T object)
{
  EGEList<T>::const_iterator it = std::find(begin(), end(), object);
  if (it != end())
  {
    erase(it);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CONTAINER_LIST_H