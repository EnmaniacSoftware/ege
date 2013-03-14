#ifndef EGE_ALIGNMENT_H
#define EGE_ALIGNMENT_H

#include "Core/Platform.h"
#include "EGEFlags.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

/*! Available alignment values. */
enum AlignmentFlags
{
  ALIGN_LEFT    = 0x01,
  ALIGN_RIGHT   = 0x02,
  ALIGN_HCENTER = 0x04,

  ALIGN_TOP     = 0x08,
  ALIGN_BOTTOM  = 0x10,
  ALIGN_VCENTER = 0x20,

  ALIGN_CENTER        = ALIGN_HCENTER | ALIGN_VCENTER,
  ALIGN_TOP_LEFT      = ALIGN_TOP | ALIGN_LEFT,
  ALIGN_TOP_RIGHT     = ALIGN_TOP | ALIGN_RIGHT,
  ALIGN_TOP_CENTER    = ALIGN_TOP | ALIGN_HCENTER,
  ALIGN_BOTTOM_LEFT   = ALIGN_BOTTOM | ALIGN_LEFT,
  ALIGN_BOTTOM_RIGHT  = ALIGN_BOTTOM | ALIGN_RIGHT,
  ALIGN_BOTTOM_CENTER = ALIGN_BOTTOM | ALIGN_HCENTER
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_FLAGS(Alignment, AlignmentFlags)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_ALIGNMENT_H