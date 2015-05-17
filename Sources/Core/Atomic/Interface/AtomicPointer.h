#ifndef EGE_CORE_ATOMIC_ATOMICPOINTER_H
#define EGE_CORE_ATOMIC_ATOMICPOINTER_H

/** Class provides platform-independent atomic operations on pointer values.
 */

#include "EGETypes.h"
#include "Core/Atomic/Interface/AtomicOperations.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
class AtomicPointer
{
  public:

    explicit AtomicPointer(T* value = 0);
    explicit AtomicPointer(const AtomicPointer& other);

  public:

    /*! Returns current value.
     *  @note This uses ordered memory model.
     */
    T* load();

    /*! Sets new value.
     *  @param  value  Value to be set.
     *  @return Initial value.
     *  @note This uses ordered memory model. 
     */
    T* store(T* value);

  private:

    /*! Current value. */
    T* m_value;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
AtomicPointer<T>::AtomicPointer(T* value)
: m_value(value)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
AtomicPointer<T>::AtomicPointer(const AtomicPointer& other)
: m_value(other.m_value)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T* AtomicPointer<T>::load()
{
  // NOTE: apparently it is not allowed to use m_value directly as 2nd and 3rd parameter of egeAtomicCompareAndSet as strange things start to happen...
  T* currentValue = m_value;

  // update the internal value
  (void) egeAtomicCompareAndSet(reinterpret_cast<void*&>(m_value), reinterpret_cast<void*>(currentValue), reinterpret_cast<void*>(currentValue));  
  return m_value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T* AtomicPointer<T>::store(T* value)
{
  return egeAtomicSet(reinterpret_cast<void*&>(m_value), reinterpret_cast<void*>(value));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_ATOMIC_ATOMICPOINTER_H