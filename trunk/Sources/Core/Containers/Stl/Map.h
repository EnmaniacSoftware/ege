#ifndef EGE_CORE_CONTAINER_MAP_H
#define EGE_CORE_CONTAINER_MAP_H

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T, typename U>
class EGEMap : public std::map<T, U>
{
  public:

    /* Returns TRUE if given key is present. */
    inline bool contains(const T& key) const;
    /* Returns value associated with given key. If key is not found, default value is returned. */
    inline const U value(const T& key, const U& defaultValue) const;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if given key is present. */
template <typename T, typename U>
bool EGEMap<T, U>::contains(const T& key) const
{
  return find(key) != end();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns value associated with given key. If key is not found, default value is returned. */
template <typename T, typename U>
const U EGEMap<T, U>::value(const T& key, const U& defaultValue) const
{
  EGEMap<T, U>::const_iterator it = find(key);
  return (it != end()) ? it->second : defaultValue;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CONTAINER_MAP_H