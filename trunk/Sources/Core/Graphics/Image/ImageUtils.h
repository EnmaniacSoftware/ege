#ifndef EGE_CORE_IMAGEUTILS_H
#define EGE_CORE_IMAGEUTILS_H

#include "Core/Graphics/Image/Image.h"
#include "EGERect.h"
#include "EGEVector2.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Image, PImage)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ImageUtils
{
  public:

    /*! Copies region from input image onto region of destination image. 
     *  @param dst     Destination image to which copying operation will be performed
     *  @param dstRect Region in destination image where data is to be copied
     *  @param src     Source image from which copy data will be taken
     *  @param srcRect Region in source image which is to be copied
     *  @note  srcRect can be NULL if entire source image is to be copied
     *  @note  If srcRect and dstRect sizes are not equal stretching is applied
     */
    static void Copy(PImage& dst, const Recti& dstRect, const PImage& src, Recti srcRect = Recti::INVALID);
    /*! Copies region from input image onto region of destination image starting at given point. 
     *  @param dst       Destination image to which copying operation will be performed
     *  @param dstPoint  Point in destination image where data is to be copied
     *  @param src       Source image from which copy data will be taken
     *  @param srcRect   Region in source image which is to be copied.
     *  @note  srcRect can be NULL if entire source image is to be copied
     */
    static void FastCopy(PImage& dst, const Vector2i& dstPoint, const PImage& src, Recti srcRect = Recti::INVALID);

    /*! Returns given format pixel size (in bytes). */
    static u32 PixelSize(PixelFormat format);

    /*! Fills region of image with given color. */
    static void Fill(PImage& dst, const Recti& dstRect, const Color& color);

    /*! Creates image from given data.
     *  @param width         Image width (in pixels).
     *  @param height        Image height (in pixels).
     *  @param format        Pixel format (in pixells).
     *  @param premultiplied TRUE if alpha premultiplication is applied.
     *  @param rowLength     Length of single row (in bytes).
     *  @param data          Pixel data buffer.
     *  @return  On success newly created image. NULL otherwise.
     */
    static PImage CreateImage(s32 width, s32 height, PixelFormat format, bool premultiplied, s32 rowLength, const PDataBuffer& data);

  public:

    /*! Performs scan line bit blit from RGBA8888 format onto RGBA8888 format. */
    static void ScanLineBltRGBA8888ToRGBA8888(void* dst, const void* src, s32 length);
    /*! Performs scan line bit blit from RGBA8888 format onto RGB888 format. */
    static void ScanLineBltRGBA8888ToRGB888(void* dst, const void* src, s32 length);
    /*! Performs fill blit on RGBA8888 surface. */
    static void FillLineBltRGBA8888(void* dst, u32 color, s32 length);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_IMAGEUTILS_H