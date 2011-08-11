#ifndef EGE_ANGLE_H
#define EGE_ANGLE_H

#include "Core/Platform.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

namespace EGEAngle
{
  /*! Available direction values. */
  enum Direction
  {
    DIRECTION_CLOCKWISE,              /*!< Clockwise direction (decreasing angle). */
    DIRECTION_COUNTERCLOCKWISE,       /*!< Counterclockwise direction (increasing angle). */ 
    DIRECTION_SHORTEST                /*!< Shortest. Can be clockwise or counterclockwise. */
  };
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "Core/Math/Angle.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_ANGLE_H