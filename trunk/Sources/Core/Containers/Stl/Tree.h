#ifndef EGE_CORE_CONTAINER_TREE_H
#define EGE_CORE_CONTAINER_TREE_H

#include "Core/Platform.h"
#include <set>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
class Tree : public std::set<T>
{
  public:

    /*! Removes given object from set. */
    void remove(const T object);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void Tree<T>::remove(const T object)
{
  typename Tree<T>::iterator it = this->find(object);
  if (it != this->end())
  {
    erase(it);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CONTAINER_TREE_H