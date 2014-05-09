#ifndef EGE_CORE_CACHE_CACHEDOBJECT_H
#define EGE_CORE_CACHE_CACHEDOBJECT_H

/*! Cached objects are wrappers around real data types with additional validity information. 
 */

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
class CachedObject
{
  public:

    CachedObject();
    CachedObject(const CachedObject<T>& other);
    explicit CachedObject(T& data);

  operators:
  
    const CachedObject& operator = (const T& data);
    operator const T&() const;

  public:

    /*! Returns TRUE if data is valid. */
    bool isValid() const;
    /*! Invalidates data. */
    void invalidate();

    /*! Returns value. */
    const T& data() const;

  private:

    /*! Cached data. */
    T m_data;
    /*! Validity flag. */
    bool m_valid;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
CachedObject<T>::CachedObject() : m_valid(false)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
CachedObject<T>::CachedObject(T& data) : m_data(data),
                                         m_valid(true)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
CachedObject<T>::CachedObject(const CachedObject<T>& other) : m_data(other.m_data),
                                                              m_valid(other.m_valid)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
const CachedObject<T>& CachedObject<T>::operator = (const T& data)
{
  m_data  = data;
  m_valid = true;

  return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
CachedObject<T>::operator const T&() const
{
  return m_data;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool CachedObject<T>::isValid() const
{
  return m_valid;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void CachedObject<T>::invalidate()
{
  m_valid = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
const T& CachedObject<T>::data() const
{
  return m_data;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CACHE_CACHEDOBJECT_H