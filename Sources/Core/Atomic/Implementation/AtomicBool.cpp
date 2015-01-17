#include "Core/Atomic/Interface/AtomicBool.h"
#include "Core/Atomic/Interface/AtomicOperations.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AtomicBool::AtomicBool(bool value) 
: m_value(value)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AtomicBool::load()
{
  return 0 != egeAtomicAdd(reinterpret_cast<volatile s32&>(m_value), 0);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool AtomicBool::store(bool value)
{
  return 0 != egeAtomicCompareAndSet(reinterpret_cast<volatile s32&>(m_value), !value, value);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END