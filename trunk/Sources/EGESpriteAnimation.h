#ifndef EGE_SPRITEANIMATION_H
#define EGE_SPRITEANIMATION_H

#include "Core/Platform.h"
#include "EGERect.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace EGESprite
{
  /*! Structure defining frame data info. */
  struct FrameData
  {
    /*! Frame rectangle on texture (in normalized local coords). */
    Rectf m_rect;
  };
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Core/Graphics/SpriteAnimation/SpriteAnimation.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_SPRITEANIMATION_H