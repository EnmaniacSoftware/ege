#ifndef EGE_CORE_IMAGEUTILS_H
#define EGE_CORE_IMAGEUTILS_H

#include "Core/Graphics/Image/Image.h"
#include <EGERect.h>
#include <EGEVector.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(Image, PImage)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ImageUtils
{
  public:

    /* Copies region from input image onto region of destination image. 
     * @param dst     Destination image to which copying operation will be performed
     * @param dstRect Region in destination image where data is to be copied
     * @param src     Source image from which copy data will be taken
     * @param srcRect Region in source image which is to be copied
     *
     * @note  srcRect can be INVALID if entire source image is to be copied
     * @note  If srcRect and dstRect sizes are not equal stretching is applied
     */
    static void Copy(PImage& dst, const Recti& dstRect, const PImage& src, Recti srcRect = Recti::INVALID);
    /* Copies region from input image onto region of destination image starting at given point. 
     * @param dst       Destination image to which copying operation will be performed
     * @param dstPoint  Point in destination image where data is to be copied
     * @param src       Source image from which copy data will be taken
     * @param srcRect   Region in source image which is to be copied.
     *
     * @note  srcRect can be INVALID if entire source image is to be copied
     */
    static void FastCopy(PImage& dst, const Vector2i& dstPoint, const PImage& src, Recti srcRect = Recti::INVALID);

    /* Returns given format pixel size (in bytes). */
    static u32 PixelSize(EGEImage::Format format);

  public:

    /* Performs scan line bit blit from RGBA8888 format onto RGBA8888 format. */
    static void ScanLineBltRGBA8888ToRGBA8888(void* dst, const void* src, s32 length);
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_IMAGEUTILS_H