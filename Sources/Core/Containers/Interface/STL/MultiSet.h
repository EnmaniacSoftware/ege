#ifndef EGE_CORE_CONTAINER_MULTISET_H
#define EGE_CORE_CONTAINER_MULTISET_H

#include "EGETypes.h"
#include <set>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
class MultiSet : public std::multiset<T>
{
  public:

    /* Removes given object from set. */
    void remove(const T object);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes given object from set. */
template <typename T>
void MultiSet<T>::remove(const T object)
{
  typename MultiSet<T>::iterator it = this->find(object);
  if (it != this->end())
  {
    erase(it);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CONTAINER_MULTISET_H