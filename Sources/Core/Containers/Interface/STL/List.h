#ifndef EGE_CORE_LIST_H
#define EGE_CORE_LIST_H

#include "EGETypes.h"
#include <list>
#include <algorithm>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
class List : public std::list<T>
{
  public:

    typedef std::list<T>::iterator Iterator;
    typedef std::list<T>::const_iterator ConstIterator;

  public:

    List();
    List(const T& object);
    List(const List& list, s32 count = -1);

  operators:

    /*! Appends given list. */
    List& operator << (const List& other);
    /*! Appends given element. */
    List& operator << (const T& value);

  public:

    /*! Returns length of the list. */
    s32 length() const;
    /*! Clears the list by removing all elements. */
    void clear();
    /*! Returns TRUE if list is empty. */
    bool isEmpty() const;

    /*! Removes element at a given index position.
     *  @param  index 0-based index at which element is to be removed from this list.
     *  @note If index is out of range no operation is performed.
     */
    void removeAt(s32 index);

    /*! Returns first element.
     *  @param  defaultValue  Value to return in case of an empty list.
     *  @return First element or default one.
     */
    const T first(const T& defaultValue) const;
    /*! Returns last element.
     *  @param  defaultValue  Value to return in case of an empty list.
     *  @return Last element or default one.
     */
    const T last(const T& defaultValue) const;

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

    /*! Returns an STL-style iterator pointing to the first item in the list. */
    Iterator begin();
    ConstIterator begin() const;

    /*! Returns an STL-style iterator pointing to the imaginary item after the last item in the list. */
    Iterator end();
    ConstIterator end() const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
List<T>::List()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
List<T>::List(const T& object)
{
  this->push_back(object);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
List<T>::List(const List<T>& list, s32 count)
{
  if (0 > count)
  {
    count = list.size();
  }

  typename List<T>::const_iterator it;
  for (it = list.begin(); (it != list.end()) && (0 < count); ++it, --count)
  {
    this->push_back(*it);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void List<T>::removeAt(s32 index)
{
  // TAGE - uncomment when EGE #101 is implemented
  // EGE_ASSERT(0 <= index);
  // EGE_ASSERT(index < size());

  if ((0 <= index) && (index < static_cast<s32>(this->size())))
  {
    typename List<T>::iterator it = this->begin();
    while (0 != index--)
    {
      ++it;
    }

    this->erase(it);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
const T List<T>::first(const T& defaultValue) const
{
  return (0 == this->size()) ? defaultValue : this->front();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
const T List<T>::last(const T& defaultValue) const
{
  return (0 == this->size()) ? defaultValue : this->back();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool List<T>::contains(const T object) const
{
  typename List<T>::const_iterator it = std::find(this->begin(), this->end(), object);
  return (it != this->end());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void List<T>::copy(const List<T>& other)
{
  this->clear();
  this->insert(this->begin(), other.begin(), other.end());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
typename List<T>::Iterator List<T>::begin()
{
  return std::list<T>::begin();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
typename List<T>::ConstIterator List<T>::begin() const
{
  return std::list<T>::begin();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
typename List<T>::Iterator List<T>::end()
{
  return std::list<T>::end();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
typename List<T>::ConstIterator List<T>::end() const
{
  return std::list<T>::end();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
List<T>& List<T>::operator << (const List<T>& other)
{
  this->insert(this->end(), other.begin(), other.end());
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
List<T>& List<T>::operator << (const T& value)
{
  this->push_back(value);
  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
s32 List<T>::length() const
{
  return static_cast<s32>(size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void List<T>::clear()
{
  std::list<T>::clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool List<T>::isEmpty() const
{
  return empty();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_LIST_H
