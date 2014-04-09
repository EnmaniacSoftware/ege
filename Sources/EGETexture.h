#ifndef EGE_TEXTURE_H
#define EGE_TEXTURE_H

#include "Core/Platform.h"
#include "Core/Graphics/Texture2D.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Available texture environment modes. */
enum TextureEnvironmentMode
{
  EM_REPLACE,                         /*< Incoming data replaces previous one. */
  EM_MODULATE,                        /*< Incoming data multiplies with previous one. Default. */
  EM_DECAL,                           /*< Incoming data replaces previous one if no ALPHA is present or blends if it is. */
  EM_BLEND,                           /*< Incoming data is blended with previous one. */
  EM_ADD,                             /*< Incoming RGB data is summed, while ALPHA channels are multiplied. */
  EM_COMBINE                          /*< Used for more complex functions definitions. NOT SUPPORTED. */
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_TEXTURE_H