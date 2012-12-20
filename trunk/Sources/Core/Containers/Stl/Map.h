#ifndef EGE_CORE_CONTAINER_MAP_H
#define EGE_CORE_CONTAINER_MAP_H

#include "Core/Platform.h"
#include <EGEDebug.h>
#include <map>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
class Map : public std::map<T, U>
{
  public:

    /* Returns TRUE if given key is present. */
    bool contains(const T& key) const;
    /*! Returns value associated with given key. If key is not found, default value is returned. */
    U value(const T& key, const U defaultValue) const;
    /*! Inserts value with given key to map. */
    void insert(const T& key, const U& value);
    /*! Returns value. */
    U& at(const T& key);
    /*! Returns value. */
    const U& at(const T& key) const;
    /*! Removes entry with a given value. Returns TRUE if entry has been removed. */
    bool removeByValue(const U value);
    /*! Removes entry with a given key. Returns TRUE if entry has been removed. */
    bool removeByKey(const T key);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
bool Map<T, U>::contains(const T& key) const
{
  return this->find(key) != this->end();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
U Map<T, U>::value(const T& key, const U defaultValue) const
{
  typename Map<T, U>::const_iterator it = this->find(key);
  return (it != this->end()) ? it->second : defaultValue;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
void Map<T, U>::insert(const T& key, const U& value)
{
  std::map<T, U>::insert(std::pair<T, U>(key, value));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
U& Map<T, U>::at(const T& key)
{
  typename Map<T, U>::iterator it = this->find(key);
  EGE_ASSERT(it != this->end());
  return it->second;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
const U& Map<T, U>::at(const T& key) const
{
  typename Map<T, U>::const_iterator it = this->find(key);
  EGE_ASSERT(it != this->end());
  return it->second;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
bool Map<T, U>::removeByValue(const U value)
{
  for (typename Map<T, U>::iterator it = this->begin(); it != this->end(); ++it)
  {
    if (it->second == value)
    {
      this->erase(it);
      return true;
    }
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
bool Map<T, U>::removeByKey(const T key)
{
  typename Map<T, U>::iterator it = this->find(key);
  if (it != this->end())
  {
    this->erase(it);
    return true;
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CONTAINER_MAP_H