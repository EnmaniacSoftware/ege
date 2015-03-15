#ifndef EGE_CORE_LIST_H
#define EGE_CORE_LIST_H

/** Lists are sequence containers that allow constant time insert and erase operations anywhere within the sequence, and iteration in both directions.
 *  Lists perform generally better in inserting, extracting and moving elements in any position within the container for which an iterator has already 
 *  been obtained, and therefore also in algorithms that make intensive use of these, like sorting algorithms.
 */

#include "EGETypes.h"
#include <list>
#include <algorithm>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
class List
{
  public:

    typedef typename std::list<T>::iterator Iterator;
    typedef typename std::list<T>::const_iterator ConstIterator;
    typedef typename std::list<T>::reverse_iterator ReverseIterator;
    typedef typename std::list<T>::const_reverse_iterator ConstReverseIterator;

  public:

    List();
    List(const T& object);
    List(const List& list, s32 count = -1);

  operators:

    /*! Appends given list. */
    List& operator << (const List& other);
    /*! Appends given element. */
    List& operator << (const T& object);

  public:

    /*! Returns length of the list. */
    s32 length() const;
    /*! Clears the list by removing all elements. */
    void clear();
    /*! Returns TRUE if list is empty. */
    bool isEmpty() const;

    /*! Removes element pointed by an iterator.
     *  @param  iterator  Iterator pointing into element to be removed from this list.
     *  @return Iterator to an element after the removed one.
     *  @note Given iterator needs to be valid. Othwise, the behavior is underfined.
     */
    Iterator remove(ConstIterator iterator);

    /*! Removes first element containing given object from this list.
     *  @param  object  Object to search for.
     */
    void remove(const T& object);

    /*! Removes the first item in the list. 
     *  @note List cannot be empty. Otherwise, behaviour is undefined.
     */
    void removeFirst();
    /*! Removes the last item in the list. 
     *  @note List cannot be empty. Otherwise, behaviour is undefined.
     */
    void removeLast();

    /*! Returns list's first element.
     *  @param  defaultValue  Value to return in case of an empty list.
     *  @return First element or default one.
     */
    const T& first(const T& defaultValue = T()) const;
    /*! Returns list's first element.
     *  @return First element of this list.
     *  @note List must contain at least one element. Otherwise, it causes undefiend behavior.
     */
    T& first();

    /*! Returns list's last element.
     *  @param  defaultValue  Value to return in case of an empty list.
     *  @return Last element or default one.
     */
    const T& last(const T& defaultValue = T()) const;
    /*! Returns list's Last element.
     *  @return Last element of this list.
     *  @note List must contain at least one element. Otherwise, it causes undefiend behavior.
     */
    T& last();

    /*! Checks for existance of the given element in this list.
     *  @param  object  Element to look for in this list.
     *  @return TRUE if given element is present is this list. Otherwise, FALSE.
     */
    bool contains(const T object) const;

    /*! Copies all data from a given list to this one.
     *  @param  from  List to copy data from.
     *  @note All data present in this list before the copy operation will be removed.
     */
    void copy(const List& from);

    /*! Adds given object to the end of the list. 
     *  @param  object  Object to be appended.
     */
    void append(const T& object);
   
    /*! Adds given object to the front of the list. 
     *  @param  object  Object to be prepended.
     */
    void prepend(const T& object);

    /*! Inserts element into the list.
     *  @param  position  Position iterator immediately before which given element should be inserted.
     *  @param  obejct    Element to be inserted.
     *  @return Iterator pointing to the inserted object.
     */
    Iterator insert(ConstIterator position, const T& object);

    /*! Returns a STL-style iterator pointing to the first item in the list. */
    Iterator begin();
    ConstIterator begin() const;

    /*! Returns a STL-style iterator pointing to the imaginary item after the last item in the list. */
    Iterator end();
    ConstIterator end() const;

    /*! Returns a STL-style iterator pointing to the first item in the list, counting from the end. */
    ReverseIterator rbegin();
    ConstReverseIterator rbegin() const;

    /*! Returns a STL-style iterator pointing to the imaginary item after the last item in the list, counting from the end. */
    ReverseIterator rend();
    ConstReverseIterator rend() const;

  private:

    /*! Underlying container. */
    std::list<T> m_data;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
List<T>::List()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
List<T>::List(const T& object)
{
  m_data.push_back(object);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
List<T>::List(const List<T>& list, s32 count)
{
  if (0 > count)
  {
    count = list.length();
  }

  ConstIterator it;
  for (it = list.begin(); (it != list.end()) && (0 < count); ++it, --count)
  {
    m_data.push_back(*it);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
typename List<T>::Iterator List<T>::remove(typename List<T>::ConstIterator iterator)
{
  return m_data.erase(iterator);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
void List<T>::remove(const T& object)
{
  m_data.remove(object);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
void List<T>::removeFirst()
{
  m_data.pop_front();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
void List<T>::removeLast()
{
  m_data.pop_back();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
T& List<T>::first()
{
  return m_data.front();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
const T& List<T>::first(const T& defaultValue) const
{
  return isEmpty() ? defaultValue : m_data.front();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
T& List<T>::last()
{
  return m_data.back();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
const T& List<T>::last(const T& defaultValue) const
{
  return isEmpty() ? defaultValue : m_data.back();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
bool List<T>::contains(const T object) const
{
  ConstIterator it = std::find(begin(), end(), object);
  return (it != end());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
void List<T>::copy(const List<T>& other)
{
  clear();
  m_data.insert(begin(), other.begin(), other.end());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
void List<T>::append(const T& object)
{
  m_data.push_back(object);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
void List<T>::prepend(const T& object)
{
  m_data.push_front(object);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
typename List<T>::Iterator List<T>::insert(typename List<T>::ConstIterator position, const T& object)
{
  return m_data.insert(position, object);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
typename List<T>::Iterator List<T>::begin()
{
  return m_data.begin();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
typename List<T>::ConstIterator List<T>::begin() const
{
  return m_data.begin();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
typename List<T>::Iterator List<T>::end()
{
  return m_data.end();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
typename List<T>::ConstIterator List<T>::end() const
{
  return m_data.end();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
typename List<T>::ReverseIterator List<T>::rbegin()
{
  return m_data.rbegin();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
typename List<T>::ConstReverseIterator List<T>::rbegin() const
{
  return m_data.rbegin();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
typename List<T>::ReverseIterator List<T>::rend()
{
  return m_data.rend();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
typename List<T>::ConstReverseIterator List<T>::rend() const
{
  return m_data.rend();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
List<T>& List<T>::operator << (const List<T>& other)
{
  m_data.insert(end(), other.begin(), other.end());
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
List<T>& List<T>::operator << (const T& value)
{
  m_data.push_back(value);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
s32 List<T>::length() const
{
  return static_cast<s32>(m_data.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
void List<T>::clear()
{
  m_data.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
bool List<T>::isEmpty() const
{
  return m_data.empty();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_LIST_H
