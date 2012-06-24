#ifndef EGE_INPUT_H
#define EGE_INPUT_H

#include "Core/Platform.h"
#include <EGEFlags.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

/*! Available buttons. */
enum Button
{
  BUTTON_NONE = 0,
  BUTTON_LEFT,
  BUTTON_RIGHT,
  BUTTON_WHEEL_UP,
  BUTTON_WHEEL_DOWN
};

/*! Available actions. */
enum Action
{
  ACTION_BUTTON_DOWN = 0,
  ACTION_BUTTON_UP,
  ACTION_MOVE
};

/*! Available keyboard modifiers. */
enum KeyboardModifiersFlags
{
  KM_NONE  = 0x00,
  KM_SHIFT = 0x01,
  KM_ALT   = 0x02,
  KM_CTRL  = 0x04
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_FLAGS(KeyboardModifiers, KeyboardModifiersFlags)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Core/Input/Pointer.h"
#include "Core/Input/PointerData.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_INPUT_H