#ifndef EGE_ATOMIC_H
#define EGE_ATOMIC_H

#include "Core/Platform.h"
#include "EGETypes.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void egeAtomicIncrement(volatile u32& value);
void egeAtomicDecrement(volatile u32& value);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_ATOMIC_H