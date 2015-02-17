#ifndef EGE_CORE_CONTAINER_MAP_H
#define EGE_CORE_CONTAINER_MAP_H

#include "Core/Platform.h"
#include "EGEDebug.h"
#include <map>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
class Map : public std::map<T, U>
{
  public:

    /*! Checks for existance of the given key in this map.
     *  @param  key Key to look for in this map.
     *  @return TRUE if given key is present is this map. Otherwise, FALSE.
     */
    bool contains(const T& key) const;

    /*! Returns value associated with a given key.
     *  @param  key Key under which value of interest is stored.
     *  @oaram  defaultValue  Value to be returned if requested key is not found.
     *  @return Value under the given key or default one.
     */
    U value(const T& key, const U defaultValue) const;

    /*! Inserts key value pair into this map.
     *  @param  key   Key at which value is to be inserted.
     *  @param  value Value to be assiciated with the key.
     *  @note If the key already exists, no data is inserted.
     */
    void insert(const T& key, const U& value);

    /*! Merges another map into this one. 
     *  @param  other               Other map to merge with this one.
     *  @param  overrideDuplicated  If set, in case of key duplicates, original values will be overriden with new values. 
                                    Otherwise, original values will be kept.
     */
    void merge(const Map& other, bool overrideDuplicates = false);

    /*! Returns value. */
    U& at(const T& key);
    /*! Returns value. */
    const U& at(const T& key) const;

    /*! Removes first key-value entry where value is equal to the given one.
     *  @param  value Value to check for while deciding if entry is to be removed.
     *  @return TRUE if entry has been removed. Otherwise, FALSE.
     */
    bool removeByValue(const U value);
    /*! Removes key-value entry for a given key.
     *  @param  key Key of the entry to be removed.
     *  @return TRUE if entry has been removed. Otherwise, FALSE.
     */
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
void Map<T, U>::merge(const Map& other, bool overrideDuplicates)
{
  if (overrideDuplicates)
  {
    for (typename Map<T, U>::const_iterator it = other.begin(); it != other.end(); ++it)
    {
      this->operator[](it->first) = it->second;
    }
  }
  else
  {
    std::map<T, U>::insert(other.begin(), other.end());
  }
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
