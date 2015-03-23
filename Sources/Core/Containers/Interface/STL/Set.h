#ifndef EGE_CORE_CONTAINER_SET_H
#define EGE_CORE_CONTAINER_SET_H

/*! Sets are containers that store unique elements following a specific order.
 *  In a set, the value of an element also identifies it (the value is itself the key), and each value must be unique. The value of the elements in a set 
 *  cannot be modified once in the container (the elements are always const), but they can be inserted or removed from the container.
 *
 *  Internally, the elements in a set are always sorted following a specific strict weak ordering criterion indicated by its internal comparison object.
 */

#include "EGETypes.h"
#include <set>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
class Set
{
  public:

    typedef typename std::set<T>::iterator Iterator;
    typedef typename std::set<T>::const_iterator ConstIterator;
    typedef typename std::set<T>::reverse_iterator ReverseIterator;
    typedef typename std::set<T>::const_reverse_iterator ConstReverseIterator;

  public:

    Set();

  public:

    /*! Returns size of the container. */
    s32 size() const;
    /*! Clears the container by removing all data. */
    void clear();
    /*! Returns TRUE if container is empty. */
    bool isEmpty() const;

    /*! Checks for existance of the value in the container.
     *  @param  value Value to look for.
     *  @return TRUE if given value is present is this container. Otherwise, FALSE.
     */
    bool contains(const T& value) const;

    /*! Inserts value into this container.
     *  @param  value Value to be inserted.
     *  @note If the value already exists, no data is inserted.
     */
    void insert(const T& value);

    /*! Removes value from a container. 
     *  @param  value Value to be removed.
     */
    void remove(const T& value);

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
    std::set<T> m_data;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
Set<T>::Set()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
s32 Set<T>::size() const
{
  return m_data.size();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void Set<T>::clear()
{
  m_data.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool Set<T>::isEmpty() const
{
  return m_data.empty();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool Set<T>::contains(const T& value) const
{
  typename std::set<T>::const_iterator it = m_data.find(value);
  return (it != m_data.end());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void Set<T>::insert(const T& value)
{
  m_data.insert(value);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void Set<T>::remove(const T& value)
{
  typename std::set<T>::iterator it = m_data.find(value);
  if (it != m_data.end())
  {
    m_data.erase(it);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
typename Set<T>::Iterator Set<T>::begin()
{
  return m_data.begin();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
typename Set<T>::ConstIterator Set<T>::begin() const
{
  return m_data.begin();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
typename Set<T>::Iterator Set<T>::end()
{
  return m_data.end();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
typename Set<T>::ConstIterator Set<T>::end() const
{
  return m_data.end();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
typename Set<T>::ReverseIterator Set<T>::rbegin()
{
  return m_data.rbegin();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
typename Set<T>::ConstReverseIterator Set<T>::rbegin() const
{
  return m_data.rbegin();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
typename Set<T>::ReverseIterator Set<T>::rend()
{
  return m_data.rend();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
typename Set<T>::ConstReverseIterator Set<T>::rend() const
{
  return m_data.rend();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CONTAINER_SET_H