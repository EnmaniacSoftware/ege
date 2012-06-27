#ifndef EGE_IMAGE_H
#define EGE_IMAGE_H

#include "Core/Platform.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
enum PixelFormat
{
  PF_UNKNOWN = -1,
  PF_RGBA_8888,           /*!< Uncompressed RGBA8888. */
  PF_RGB_888,             /*!< Uncompressed RGB888. */
  PF_RGBA_5551,           /*!< Uncompressed RGBA5551. */
  PF_RGBA_4444,           /*!< Uncompressed RGBA4444. */
  PF_RGB_565,             /*!< Uncompressed RGB565. */
  PF_PVRTC_RGB_2BPP,      /*!< PVRTC 2 bits per pixel compressed RGB image. */
  PF_PVRTC_RGB_4BPP,      /*!< PVRTC 4 bits per pixel compressed RGB image. */
  PF_PVRTC_RGBA_2BPP,     /*!< PVRTC 2 bits per pixel compressed RGBA image. */
  PF_PVRTC_RGBA_4BPP      /*!< PVRTC 4 bits per pixel compressed RGBA image. */
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Core/Graphics/Image/Image.h"
#include "Core/Graphics/Image/ImageUtils.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_IMAGE_H