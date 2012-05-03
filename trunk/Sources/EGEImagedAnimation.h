#ifndef EGE_IMAGEDANIMATION_H
#define EGE_IMAGEDANIMATION_H

#include "Core/Platform.h"
#include <EGEMatrix.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace EGEImagedAnimation
{
  /*! Structure defining frame data info. */
  struct ActionData
  {
    s32 objectId;                             /*!< Object ID for which action is to be performed. */
    s32 queue;                                /*!< Render queue offset. */
    Matrix4f matrix;                          /*!< Transformation matrix. */
  };
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Core/Graphics/ImagedAnimation/ImagedAnimation.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_IMAGEDANIMATION_H