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
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CONTAINER_TREE_H