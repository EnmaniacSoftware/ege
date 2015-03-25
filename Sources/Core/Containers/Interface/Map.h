#ifndef EGE_CORE_CONTAINER_MAP_H
#define EGE_CORE_CONTAINER_MAP_H

/*! Maps are associative containers that store elements formed by a combination of a key value and a mapped value, following a specific order.
 *  In a map, the key values are generally used to sort and uniquely identify the elements, while the mapped values store the content associated to this key. 
 *
 *  Internally, the elements in a map are always sorted by its key following a specific strict weak ordering criterion indicated by its internal comparison 
 *  object.
 */

#include "EGEDebug.h"
#include "EGETypes.h"
#include <map>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
class Map
{
  public:

    typedef typename std::map<T, U>::iterator Iterator;
    typedef typename std::map<T, U>::const_iterator ConstIterator;
    typedef typename std::map<T, U>::reverse_iterator ReverseIterator;
    typedef typename std::map<T, U>::const_reverse_iterator ConstReverseIterator;

  public:

    Map();

  operators:

    /*! Returns reference to a value associated by a given key. If such key does not exists it is added to the container with default contructed value.
     *  @param  key Key under which value of interest is stored.
     *  @return Value under the given key or default constructed one.
     */
    U& operator[](const T& key);

  public:

    /*! Returns size of the container. */
    s32 size() const;
    /*! Clears the container by removing all data. */
    void clear();
    /*! Returns TRUE if container is empty. */
    bool isEmpty() const;

    /*! Checks for existance of the given key in this map.
     *  @param  key Key to look for in this map.
     *  @return TRUE if given key is present is this map. Otherwise, FALSE.
     */
    bool contains(const T& key) const;

    /*! Returns value associated with a given key.
     *  @param  key           Key under which value of interest is stored.
     *  @oaram  defaultValue  Value to be returned if requested key is not found.
     *  @return Value under the given key or default one.
     */
    const U& value(const T& key, const U& defaultValue = U()) const;

    /*! Inserts key value pair into this map.
     *  @param  key   Key at which value is to be inserted.
     *  @param  value Value to be assiciated with the key.
     *  @note If the key already exists, no data is inserted.
     */
    void insert(const T& key, const U& value);

    /*! Appends another map into this one. 
     *  @param  other               Other map to append to this one.
     *  @param  replaceDuplicates   If set, in case of key duplicates, original values accociated with them will be replaced with new ones. 
     *                              Otherwise, original values will be preserved.
     */
    void append(const Map& other, bool replaceDuplicates = false);

    /*! Removes key-value entry where value is equal to the given one.
     *  @param  value Value to check for while deciding if entry is to be removed.
     *  @return TRUE if entry has been removed. Otherwise, FALSE.
     */
    bool removeByValue(const U& value);
    /*! Removes key-value entry for a given key.
     *  @param  key Key of the entry to be removed.
     *  @return TRUE if entry has been removed. Otherwise, FALSE.
     */
    bool removeByKey(const T& key);
    /*! Removes key-value pointed by an iterator.
     *  @param  iterator  Iterator pointing into key-value element to be removed from this container.
     *  @return Iterator to an element after the removed one.
     *  @note Given iterator needs to be valid. Othwise, the behavior is underfined.
     */
    Iterator remove(ConstIterator iterator);

    /*! Returns a STL-style iterator pointing to the first item in the container. */
    Iterator begin();
    ConstIterator begin() const;

    /*! Returns a STL-style iterator pointing to the imaginary item after the last item in the container. */
    Iterator end();
    ConstIterator end() const;

    /*! Returns a STL-style iterator pointing to the first item in the container, counting from the end. */
    ReverseIterator rbegin();
    ConstReverseIterator rbegin() const;

    /*! Returns a STL-style iterator pointing to the imaginary item after the last item in the container, counting from the end. */
    ReverseIterator rend();
    ConstReverseIterator rend() const;

  private:

    /*! Underlying container. */
    std::map<T, U> m_data;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
Map<T, U>::Map()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
U& Map<T, U>::operator[](const T& key)
{
  return m_data[key];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
s32 Map<T, U>::size() const
{
  return m_data.size();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
void Map<T, U>::clear()
{
  m_data.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
bool Map<T, U>::isEmpty() const
{
  return m_data.empty();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
bool Map<T, U>::contains(const T& key) const
{
  return m_data.find(key) != m_data.end();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
const U& Map<T, U>::value(const T& key, const U& defaultValue) const
{
  typename std::map<T, U>::const_iterator it = m_data.find(key);
  return (it != m_data.end()) ? it->second : defaultValue;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
void Map<T, U>::insert(const T& key, const U& value)
{
  m_data.insert(std::pair<T, U>(key, value));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
void Map<T, U>::append(const Map& other, bool replaceDuplicates)
{
  if (replaceDuplicates)
  {
    for (typename std::map<T, U>::const_iterator it = other.m_data.begin(); it != other.m_data.end(); ++it)
    {
      m_data[it->first] = it->second;
    }
  }
  else
  {
    m_data.insert(other.m_data.begin(), other.m_data.end());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
bool Map<T, U>::removeByValue(const U& value)
{
  for (typename std::map<T, U>::iterator it = m_data.begin(); it != m_data.end(); ++it)
  {
    if (it->second == value)
    {
      m_data.erase(it);
      return true;
    }
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
bool Map<T, U>::removeByKey(const T& key)
{
  typename std::map<T, U>::iterator it = m_data.find(key);
  if (it != m_data.end())
  {
    m_data.erase(it);
    return true;
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
typename Map<T, U>::Iterator Map<T, U>::remove(typename Map<T, U>::ConstIterator iterator)
{
  return m_data.erase(iterator);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
typename Map<T, U>::Iterator Map<T, U>::begin()
{
  return m_data.begin();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
typename Map<T, U>::ConstIterator Map<T, U>::begin() const
{
  return m_data.begin();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
typename Map<T, U>::Iterator Map<T, U>::end()
{
  return m_data.end();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
typename Map<T, U>::ConstIterator Map<T, U>::end() const
{
  return m_data.end();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
typename Map<T, U>::ReverseIterator Map<T, U>::rbegin()
{
  return m_data.rbegin();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
typename Map<T, U>::ConstReverseIterator Map<T, U>::rbegin() const
{
  return m_data.rbegin();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
typename Map<T, U>::ReverseIterator Map<T, U>::rend()
{
  return m_data.rend();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
typename Map<T, U>::ConstReverseIterator Map<T, U>::rend() const
{
  return m_data.rend();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CONTAINER_MAP_H
