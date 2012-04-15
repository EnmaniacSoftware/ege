#ifndef EGE_IMAGEDANIMATION_H
#define EGE_IMAGEDANIMATION_H

#include "Core/Platform.h"
#include <EGEMatrix.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

namespace EGEImagedAnimation
{
  /*! Available actions. */
  //enum Action
  //{
  //  ACTION_SHOW,
  //  ACTION_HIDE,
  //  ACTION_UPDATE
  //};

  /*! Structure defining frame data info. */
  struct ActionData
  {
    s32 queue;
    Matrix4f matrix;
  };
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "Core/Animation/ImagedAnimation.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_IMAGEDANIMATION_H