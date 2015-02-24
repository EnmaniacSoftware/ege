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

    /*! Inserts key value pair into this map.
     *  @param  key   Key at which value is to be inserted.
     *  @param  value Value to be assiciated with the key.
     */
    void insert(const T& key, const U& value);

    /*! Fills given list with all objects of a given key. */
    // TAGE - move to some utility class to remove dependency to List ?
    void values(const T& key, List<U>& list) const;

    /*! Removes first key-value entry where value is equal to the given one.
     *  @param  value Value to check for while deciding if entry is to be removed.
     *  @return TRUE if entry has been removed. Otherwise, FALSE.
     */
    bool removeByValue(const U value);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
void MultiMap<T, U>::insert(const T& key, const U& value)
{
  std::multimap<T, U>::insert(std::pair<T, U>(key, value));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
void MultiMap<T, U>::values(const T& key, List<U>& list) const
{
  // retrieve all entries with a given key
  std::pair<typename std::multimap<T, U>::const_iterator, typename std::multimap<T, U>::const_iterator> range = this->equal_range(key);

  // add values into list
  for (typename std::multimap<T, U>::const_iterator it = range.first; it != range.second; ++it)
  {
    list.append(it->second);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
bool MultiMap<T, U>::removeByValue(const U value)
{
  for (typename MultiMap<T, U>::iterator it = this->begin(); it != this->end(); ++it)
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

EGE_NAMESPACE_END

#endif // EGE_CORE_CONTAINER_MULTIMAP_H
