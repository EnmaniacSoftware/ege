#ifndef EGE_CORE_CONTAINER_DYNAMICARRAY_H
#define EGE_CORE_CONTAINER_DYNAMICARRAY_H

/** Dynamic arrays are sequence containers representing arrays that can change in size.
 *  Just like arrays, they use contiguous storage locations for their elements, which means that their elements can also be accessed using offsets on regular 
 *  pointers to its elements, and just as efficiently as in arrays. But unlike arrays, their size can change dynamically, with their storage being handled 
 *  automatically by the container.
 *
 *  Internal storage may need to be reallocated in order to grow in size when new elements are inserted, which implies allocating a new storage and moving all 
 *  elements to it. This is a relatively expensive task in terms of processing time, and thus, dynamic arrays do not reallocate each time an element is added 
 *  to the container.
 *
 *  Instead, dynamic array containers may allocate some extra storage to accommodate for possible growth, and thus the container may have an actual capacity 
 *  greater than the storage strictly needed to contain its elements (i.e., its length). 
 *
 *  Compared to the other dynamic sequence containers (lists), dynamic arrays are very efficient accessing its elements (just like arrays) and relatively 
 *  efficient adding or removing elements from its end. For operations that involve inserting or removing elements at positions other than the end, they 
 *  perform worse than the others, and have less consistent iterators and references than lists.
 */

#include "EGETypes.h"
#include <vector>
#include <algorithm>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
class DynamicArray
{
  public:

    typedef typename std::vector<T>::iterator Iterator;
    typedef typename std::vector<T>::const_iterator ConstIterator;
    typedef typename std::vector<T>::reverse_iterator ReverseIterator;
    typedef typename std::vector<T>::const_reverse_iterator ConstReverseIterator;
    typedef typename std::vector<T>::reference Reference;
    typedef typename std::vector<T>::const_reference ConstReference;

  public:

    DynamicArray();
    DynamicArray(const T& object);

  operators:

    DynamicArray& operator << (const DynamicArray& other);
    DynamicArray& operator << (const T& value);

    /*! Returns reference to the object at a given position.
     *  @param  position  0-based index of the element to access.
     *  @return Reference to the element.
     *  @note Position should be in range. Otherwise, behaviour is undefined.
     */
	  ConstReference operator[](s32 position) const;
	  Reference      operator[](s32 position);

    bool           operator==(const DynamicArray<T>& other) const;

  public:

    /*! Clears the container by removing all elements. */
    void clear();
    /*! Returns TRUE if container is empty. */
    bool isEmpty() const;

    /*! Reserves storage for a given number of elements. 
     *  @param  count Number of elements storage is to be allocated for.
     *  @note If requested storage space is greater than current capacity reallocation is done. Otherwise, no action is performed.
     *        Changing the capacity do not affect array's elements.
     */
    void setCapacity(s32 count);

    /*! Resizes the container so that it contains given amount of elements.
     *  @param  count Number of elements to be present in the container.
     *  @note If requested number is smaller than the current container size, the content is reduced to its first count elements,
     *        removing those beyond (and destroying them). 
     *        If requested number is greater than the current container size, the content is expanded by inserting at the end as many elements as needed 
     *        to reach requested size. 
     *        If requested number is also greater than the current container capacity, an automatic reallocation of the allocated storage space takes place.
     */
    void setLength(s32 count);
    /*! Returns length of the container. */
    s32 length() const;

    /*! Removes element from this container. 
     *  @param  object  Element to be removed.
     */
    void remove(const T object);
    /*! Removes element at given index. 
     *  @param  index 0-based index of the element to be removed from this container.
     *  @note If index if out of range, no action is performed.
     */
    void removeAt(s32 index);
    /*! Removes element pointed by an iterator.
     *  @param  iterator  Iterator pointing into element to be removed from this array.
     *  @return Iterator to an element after the removed one.
     *  @note Given iterator needs to be valid. Othwise, the behavior is underfined.
     */
    Iterator remove(ConstIterator iterator);
    /*! Removes the last item in the container. 
     *  @note Container cannot be empty. Otherwise, behaviour is undefined.
     */
    void removeLast();

    /*! Returns containers's first element.
     *  @param  defaultValue  Value to return in case of an empty container.
     *  @return First element or default one.
     */
    ConstReference first(const T& defaultValue = T()) const;
    /*! Returns containers's first element.
     *  @return First element of this container.
     *  @note Container must contain at least one element. Otherwise, it causes undefiend behavior.
     */
    Reference first();

    /*! Returns last element. 
     *  @param  defaultValue  Default value to be returned in case of an empty container. 
     *  @return Last element or default value.
     */
    ConstReference last(const T& defaultValue = T()) const;

    /*! Returns container's last element.
     *  @return Last element of this container.
     *  @note Container must contain at least one element. Otherwise, behavior is undefined.
     */
    Reference last();

    /*! Returns TRUE if the given element is present in this container. 
     *  @param  object  Element to be checked for existance.
     *  @return TRUE if given element is present. Otherwise, FALSE.
     */
    bool contains(const T object) const;

    /*! Copies all data from a given container into the current one. 
     *  @param  from  Container data is copied from.
     *  @note All old data in the current container is removed before the copy operation.
     */
    void copy(const DynamicArray& from);

    /*! Adds given object to the end of the container. 
     *  @param  object  Object to be appended.
     */
    void append(const T& object);

    /*! Returns element at given position. 
     *  @param  index         0-based index of the element to be returned from this container.
     *  @param  defaultValue  Default value to be returned in case no element could be found. 
     *  @return Found element or default one.
     */
    ConstReference at(s32 index, const T& defaultValue) const;

    /*! Returns an index of first occurence of a given element. 
     *  @param  object  Object to test for occurence.
     *  @return 0-based index position of the given element within this container. Negative value is returned if it is not found.
     */
    s32 indexOf(const T& object) const;

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
    std::vector<T> m_data;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
DynamicArray<T>::DynamicArray()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
DynamicArray<T>::DynamicArray(const T& object)
{
  m_data.push_back(object);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
typename DynamicArray<T>::ConstReference DynamicArray<T>::operator[](s32 position) const
{
  return m_data[position];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
typename DynamicArray<T>::Reference DynamicArray<T>::operator[](s32 position)
{
  return m_data[position];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool DynamicArray<T>::operator==(const DynamicArray<T>& other) const
{
  return m_data == other.m_data;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void DynamicArray<T>::remove(const T object)
{
  typename std::vector<T>::iterator it = std::find(m_data.begin(), m_data.end(), object);
  if (it != m_data.end())
  {
    m_data.erase(it);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void DynamicArray<T>::removeAt(s32 index)
{
// TAGE - uncomment once EGE #110 is done
//  EGE_ASSERT(0 <= index);
//  EGE_ASSERT(index < size());

  if ((0 <= index) && (index < length()))
  {
    typename std::vector<T>::iterator it = m_data.begin() + index;
    m_data.erase(it);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
typename DynamicArray<T>::Iterator DynamicArray<T>::remove(typename DynamicArray<T>::ConstIterator iterator)
{
  return m_data.erase(iterator);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
void DynamicArray<T>::removeLast()
{
  m_data.pop_back();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
typename DynamicArray<T>::Reference DynamicArray<T>::first()
{
  return m_data.front();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
typename DynamicArray<T>::ConstReference DynamicArray<T>::first(const T& defaultValue) const
{
  return isEmpty() ? defaultValue : m_data.front();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
typename DynamicArray<T>::ConstReference DynamicArray<T>::last(const T& defaultValue) const
{
  return at(length() - 1, defaultValue);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
typename DynamicArray<T>::Reference DynamicArray<T>::last()
{
  return m_data.back();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool DynamicArray<T>::contains(const T object) const
{
  typename std::vector<T>::const_iterator it = std::find(m_data.begin(), m_data.end(), object);
  return (it != m_data.end());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void DynamicArray<T>::copy(const DynamicArray<T>& from)
{
  m_data.clear();
  m_data.insert(m_data.begin(), from.m_data.begin(), from.m_data.end());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
void DynamicArray<T>::append(const T& object)
{
  m_data.push_back(object);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
DynamicArray<T>& DynamicArray<T>::operator << (const DynamicArray<T>& other)
{
  m_data.insert(m_data.end(), other.m_data.begin(), other.m_data.end());
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
DynamicArray<T>& DynamicArray<T>::operator << (const T& value)
{
  m_data.push_back(value);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
typename DynamicArray<T>::ConstReference DynamicArray<T>::at(s32 index, const T& defaultValue) const
{
  if ((0 <= index) && (index < length()))
  {
    return m_data[(static_cast<size_t>(index))];
  }

  return defaultValue;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
s32 DynamicArray<T>::indexOf(const T& object) const
{
  s32 i = 0;
  for (typename std::vector<T>::const_iterator it = m_data.begin(); it != m_data.end(); ++it, ++i)
  {
    if (*it == object)
    {
      // found
      return i;
    }
  }

  // not found
  return -1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
s32 DynamicArray<T>::length() const
{
  return static_cast<s32>(m_data.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
void DynamicArray<T>::clear()
{
  m_data.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
bool DynamicArray<T>::isEmpty() const
{
  return m_data.empty();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
void DynamicArray<T>::setCapacity(s32 count)
{
  m_data.reserve(count);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
void DynamicArray<T>::setLength(s32 count)
{
  m_data.resize(count);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
typename DynamicArray<T>::Iterator DynamicArray<T>::begin()
{
  return m_data.begin();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
typename DynamicArray<T>::ConstIterator DynamicArray<T>::begin() const
{
  return m_data.begin();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
typename DynamicArray<T>::Iterator DynamicArray<T>::end()
{
  return m_data.end();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
typename DynamicArray<T>::ConstIterator DynamicArray<T>::end() const
{
  return m_data.end();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
typename DynamicArray<T>::ReverseIterator DynamicArray<T>::rbegin()
{
  return m_data.rbegin();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
typename DynamicArray<T>::ConstReverseIterator DynamicArray<T>::rbegin() const
{
  return m_data.rbegin();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
typename DynamicArray<T>::ReverseIterator DynamicArray<T>::rend()
{
  return m_data.rend();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
typename DynamicArray<T>::ConstReverseIterator DynamicArray<T>::rend() const
{
  return m_data.rend();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CONTAINER_DYNAMICARRAY_H