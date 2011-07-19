#include "Core/Graphics/Image/ImageUtils.h"
#include <EGEDataBuffer.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef void (*PFNSCANLINEBLTFUNC) (void* dst, const void* src, s32 length);

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

struct ScanLineEntry
{
  EGEImage::Format dstFormat;
  EGEImage::Format srcFormat;

  PFNSCANLINEBLTFUNC scanline;
};

static struct ScanLineEntry ScanLines[] = { {EGEImage::RGBA_8888, EGEImage::RGBA_8888, ImageUtils::ScanLineBltRGBA8888ToRGBA8888}
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns given format pixel size (in bytes). */
u32 ImageUtils::PixelSize(EGEImage::Format format)
{
  switch (format)
  {
    case EGEImage::RGB_888:   return 3;
    case EGEImage::RGBA_8888: return 4;
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Copies region from input image onto region of destination image. 
 * @param dst     Destination image to which copying operation will be performed
 * @param dstRect Region in destination image where data is to be copied
 * @param src     Source image from which copy data will be taken
 * @param srcRect Region in source image which is to be copied
 *
 * @note  srcRect can be INVALID if entire source image is to be copied
 * @note  If srcRect and dstRect sizes are not equal stretching is applied
 */
void ImageUtils::Copy(PImage& dst, const Recti& dstRect, const PImage& src, Recti srcRect)
{
  // check if entire source should be copied
  if (srcRect.isInvalid())
  {
    srcRect = Recti(0, 0, src->width(), src->height());
  }

  // TAGE - implement rest of logic
  Vector2i point(dstRect.x, dstRect.y);
  FastCopy(dst, point, src, srcRect);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Copies region from input image onto region of destination image starting at given point. 
 * @param dst       Destination image to which copying operation will be performed
 * @param dstPoint  Point in destination image where data is to be copied
 * @param src       Source image from which copy data will be taken
 * @param srcRect   Region in source image which is to be copied.
 *
 * @note  srcRect can be INVALID if entire source image is to be copied
 */
void ImageUtils::FastCopy(PImage& dst, const Vector2i& dstPoint, const PImage& src, Recti srcRect)
{
  // check if entire source should be copied
  if (srcRect.isInvalid())
  {
    srcRect = Recti(0, 0, src->width(), src->height());
  }

  // TAGE - clipping to be done
  Recti dstRect(dstPoint.x, dstPoint.y, srcRect.width, srcRect.height);
  //dstRect = Recti(0, 0, dst->width(), dst->height()).

  PFNSCANLINEBLTFUNC scanline = NULL;
  for (u32 i = 0; i < sizeof (ScanLines) / sizeof (ScanLines[0]); ++i)
  {
    // check if proper entry found
    if ((ScanLines[i].dstFormat == dst->format()) && (ScanLines[i].srcFormat == src->format()))
    {
      // found
      scanline = ScanLines[i].scanline;
      break;
    }
  }

  if (scanline)
  {
    void* dstLine = reinterpret_cast<u8*>(dst->data()->data()) + dstRect.y * dst->m_rowLength + dstRect.x * PixelSize(dst->format());
    void* srcLine = reinterpret_cast<u8*>(src->data()->data()) + srcRect.y * src->m_rowLength + srcRect.x * PixelSize(src->format());

    for (s32 y = 0; y < srcRect.height; ++y)
    {
      scanline(dstLine, srcLine, dstRect.width);

      dstLine = reinterpret_cast<u8*>(dstLine) + dst->m_rowLength;
      srcLine = reinterpret_cast<u8*>(srcLine) + src->m_rowLength;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Performs scan line bit blit from RGBA8888 format onto RGBA8888 format. */
void ImageUtils::ScanLineBltRGBA8888ToRGBA8888(void* dst, const void* src, s32 length)
{
  EGE_MEMCPY(dst, src, length * 4);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
