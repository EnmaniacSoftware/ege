#ifndef EGE_CORE_CONTAINER_MULTIMAP_H
#define EGE_CORE_CONTAINER_MULTIMAP_H

/*! Multimaps are associative containers that store elements formed by a combination of a key value and a mapped value, following a specific order, and where 
 *  multiple elements can have equivalent keys.
 *
 *  In a multimap, the key values are generally used to sort and uniquely identify the elements, while the mapped values store the content associated to this 
 *  key. 
 * 
 *  Internally, the elements in a multimap are always sorted by its key following a specific strict weak ordering criterion indicated by its internal 
 *  comparison object.
 */

#include "Core/Platform.h"
#include "EGEList.h"
#include <map>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
class MultiMap
{
  public:

    typedef typename std::multimap<T, U>::iterator Iterator;
    typedef typename std::multimap<T, U>::const_iterator ConstIterator;
    typedef typename std::multimap<T, U>::reverse_iterator ReverseIterator;
    typedef typename std::multimap<T, U>::const_reverse_iterator ConstReverseIterator;

  public:

    MultiMap();

  public:

    /*! Returns size of the container. */
    s32 size() const;
    /*! Clears the container by removing all data. */
    void clear();
    /*! Returns TRUE if container is empty. */
    bool isEmpty() const;

    /*! Inserts key value pair into this map.
     *  @param  key   Key at which value is to be inserted.
     *  @param  value Value to be assiciated with the key.
     */
    void insert(const T& key, const U& value);

    /*! Returns the list of values associated with a given key. 
     *  @reutrn List of values.
     */
    // TAGE - move to some utility class to remove dependency to List ?
    List<U> values(const T& keylist) const;

    /*! Removes first key-value entry where value is equal to the given one.
     *  @param  value Value to check for while deciding if entry is to be removed.
     *  @return TRUE if entry has been removed. Otherwise, FALSE.
     */
    bool removeByValue(const U value);
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
    std::multimap<T, U> m_data;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
MultiMap<T, U>::MultiMap()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
s32 MultiMap<T, U>::size() const
{
  return m_data.size();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
void MultiMap<T, U>::clear()
{
  m_data.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
bool MultiMap<T, U>::isEmpty() const
{
  return m_data.empty();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
void MultiMap<T, U>::insert(const T& key, const U& value)
{
  m_data.insert(std::pair<T, U>(key, value));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
List<U> MultiMap<T, U>::values(const T& key) const
{
  List<U> list;

  // retrieve all entries with a given key
  std::pair<typename std::multimap<T, U>::const_iterator, typename std::multimap<T, U>::const_iterator> range = m_data.equal_range(key);

  // add values into list
  for (typename std::multimap<T, U>::const_iterator it = range.first; it != range.second; ++it)
  {
    list.append(it->second);
  }

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
bool MultiMap<T, U>::removeByValue(const U value)
{
  for (typename std::multimap<T, U>::iterator it = m_data.begin(); it != m_data.end(); ++it)
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
typename MultiMap<T, U>::Iterator MultiMap<T, U>::remove(typename MultiMap<T, U>::ConstIterator iterator)
{
  return m_data.erase(iterator);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
typename MultiMap<T, U>::Iterator MultiMap<T, U>::begin()
{
  return m_data.begin();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
typename MultiMap<T, U>::ConstIterator MultiMap<T, U>::begin() const
{
  return m_data.begin();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
typename MultiMap<T, U>::Iterator MultiMap<T, U>::end()
{
  return m_data.end();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
typename MultiMap<T, U>::ConstIterator MultiMap<T, U>::end() const
{
  return m_data.end();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
typename MultiMap<T, U>::ReverseIterator MultiMap<T, U>::rbegin()
{
  return m_data.rbegin();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
typename MultiMap<T, U>::ConstReverseIterator MultiMap<T, U>::rbegin() const
{
  return m_data.rbegin();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
typename MultiMap<T, U>::ReverseIterator MultiMap<T, U>::rend()
{
  return m_data.rend();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T, typename U>
typename MultiMap<T, U>::ConstReverseIterator MultiMap<T, U>::rend() const
{
  return m_data.rend();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CONTAINER_MULTIMAP_H
