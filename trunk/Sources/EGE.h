#ifndef EGE_H
#define EGE_H

#include "Core/Platform.h"
#include "Core/ObjectUIDs.h"

#include <EGEMemory.h>
#include <EGETypes.h>
#include <EGEFlags.h>

#include "Core/ListenerContainer.h"
#include "Core/ConfigParams.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

/*! Available directions. */
enum DirectionFlags
{
  DIRECTION_NONE        = 0x00,
  DIRECTION_VERTICAL    = 0x01,
  DIRECTION_HORIZONTAL  = 0x02,

  DIRECTION_BOTH = DIRECTION_VERTICAL | DIRECTION_HORIZONTAL
};

EGE_DECLARE_FLAGS(Direction, DirectionFlags)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_H