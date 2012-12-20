#ifndef EGE_CORE_CONTAINER_MULTIMAP_H
#define EGE_CORE_CONTAINER_MULTIMAP_H

#include "Core/Platform.h"
#include <map>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
class MultiMap : public std::multimap<T, U>
{
  public:

    /*! Inserts value with given key to map. */
    void insert(const T& key, const U& value);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
void MultiMap<T, U>::insert(const T& key, const U& value)
{
  std::multimap<T, U>::insert(std::pair<T, U>(key, value));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CONTAINER_MULTIMAP_H