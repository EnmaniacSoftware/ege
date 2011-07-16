#ifndef EGE_TEXTURE_H
#define EGE_TEXTURE_H

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
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "Core/Graphics/Texture2D.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_TEXTURE_H