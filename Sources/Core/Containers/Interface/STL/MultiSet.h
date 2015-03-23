#ifndef EGE_CORE_CONTAINER_MULTITREE_H
#define EGE_CORE_CONTAINER_MULTITREE_H

#include "Core/Platform.h"
#include <set>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class MultiTree : public std::multiset<T>
{
  public:

    /* Removes given object from set. */
    void remove(const T object);
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes given object from set. */
template <typename T>
void MultiTree<T>::remove(const T object)
{
  typename MultiTree<T>::iterator it = this->find(object);
  if (it != this->end())
  {
    erase(it);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CONTAINER_MULTITREE_H