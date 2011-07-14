#ifndef EGE_CORE_CONTAINER_MAP_H
#define EGE_CORE_CONTAINER_MAP_H

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "Core/Platform.h"
#include <map>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T, typename U>
class EGEMap : public std::map<T, U>
{
  public:

    /* Returns TRUE if given key is present. */
    inline bool contains(const T& key) const;
    /* Returns value associated with given key. If key is not found, default value is returned. */
    inline U value(const T& key, const U& defaultValue) const;
    /* Inserts value with given key to map. */
    inline void insert(const T& key, const U& value);
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if given key is present. */
template <typename T, typename U>
bool EGEMap<T, U>::contains(const T& key) const
{
  return this->find(key) != this->end();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns value associated with given key. If key is not found, default value is returned. */
template <typename T, typename U>
U EGEMap<T, U>::value(const T& key, const U& defaultValue) const
{
  typename EGEMap<T, U>::const_iterator it = this->find(key);
  return (it != this->end()) ? it->second : defaultValue;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Inserts value with given key to map. */
template <typename T, typename U>
void EGEMap<T, U>::insert(const T& key, const U& value)
{
  std::map<T, U>::insert(std::pair<T, U>(key, value));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CONTAINER_MAP_H