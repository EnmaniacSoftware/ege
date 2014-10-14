#ifndef EGE_CORE_KEYMODIFIERS_H
#define EGE_CORE_KEYMODIFIERS_H

#include "EGE.h"
#include "EGEFlags.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Available keyboard modifiers. */
enum KeyboardModifiersFlags
{
  EKeyModifierNone  = 0x00,
  EKeyModifierShift = 0x01,
  EKeyModifierAlt   = 0x02,
  EKeyModifierCtrl  = 0x04
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_FLAGS(KeyboardModifiers, KeyboardModifiersFlags)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_KEYMODIFIERS_H