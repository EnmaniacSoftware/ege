#ifndef EGE_TEXTURE_H
#define EGE_TEXTURE_H

#include "Core/Platform.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

namespace EGETexture
{
    /*! Texture filters. */
    enum Filter
    {
      BILINEAR = 0,                       /*< for MIN and MAG. */
      TRILINEAR,                          /*< for MIN and MAG. */
      MIPMAP_BILINEAR,                    /*< for MIN only. */
      MIPMAP_TRILINEAR,                   /*< for MIN only. */
    };
  
    /*! Texture addressing modes. */
    enum AddressingMode
    {
      AM_CLAMP  = 0,                      /*< Texture clapms at values over 1.0. */
      AM_REPEAT                           /*< Texture repeats at values over 1.0. */
    };

    /*! Texture environment modes. */
    enum EnvironmentMode
    {
      EM_REPLACE,                         /*< Incoming data replaces previous one. */
      EM_MODULATE,                        /*< Incoming data multiplies with previous one. Default. */
      EM_DECAL,                           /*< Incoming data replaces previous one if no ALPHA is present or blends if it is. */
      EM_BLEND,                           /*< Incoming data is blended with previous one. */
      EM_ADD,                             /*< Incoming RGB data is summed, while ALPHA channels are multiplied. */
      EM_COMBINE                          /*< Used for more complex functions definitions. NOT SUPPORTED. */
    };
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "Core/Graphics/Texture2D.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_TEXTURE_H