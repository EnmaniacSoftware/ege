#include "Core/Graphics/Image/ImageUtils.h"
#include "EGEDataBuffer.h"
#include "EGEColor.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef void (*PFNSCANLINEBLTFUNC) (void* dst, const void* src, s32 length);
typedef void (*PFNFILLLINEBLTFUNC) (void* dst, u32 color, s32 length);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
struct ScanLineEntry
{
  PixelFormat dstFormat;
  PixelFormat srcFormat;

  PFNSCANLINEBLTFUNC scanline;
};

static struct ScanLineEntry ScanLines[] = { {PF_RGBA_8888, PF_RGBA_8888, ImageUtils::ScanLineBltRGBA8888ToRGBA8888},
                                            {PF_RGB_888, PF_RGBA_8888, ImageUtils::ScanLineBltRGBA8888ToRGB888}
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 ImageUtils::PixelSize(PixelFormat format)
{
  switch (format)
  {
    case PF_RGB_888:   return 3;
    case PF_RGBA_8888: return 4;
    
    default:
    
      return 0;
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImageUtils::Copy(PImage& dst, const Recti& dstRect, const PImage& src, Recti srcRect)
{
  // check if entire source should be copied
  if (srcRect.isNull())
  {
    srcRect = Recti(0, 0, src->width(), src->height());
  }

  // TAGE - implement rest of logic
  Vector2i point(dstRect.x, dstRect.y);
  FastCopy(dst, point, src, srcRect);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImageUtils::FastCopy(PImage& dst, const Vector2i& dstPoint, const PImage& src, Recti srcRect)
{
  // check if entire source should be copied
  if (srcRect.isNull())
  {
    srcRect = Recti(0, 0, src->width(), src->height());
  }

  // ideally destination rectagle starts at destination point and is of source rect dimensions
  Recti dstRect(dstPoint.x, dstPoint.y, srcRect.width, srcRect.height);

  // clip source and destination rectangles to corresponding image surface dimensions
  Recti dstImgRect(0, 0, dst->width(), dst->height());
  Recti srcImgRect(0, 0, src->width(), src->height());

  srcRect = srcImgRect.intersect(srcRect);
  dstRect = dstImgRect.intersect(dstRect);

  // find correct scanline blitter
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

  // check if blitter found
  if (scanline)
  {
    // get first line beginings in destination and source buffers
    void* dstLine = reinterpret_cast<u8*>(dst->data()->data()) + dstRect.y * dst->m_rowLength + dstRect.x * PixelSize(dst->format());
    void* srcLine = reinterpret_cast<u8*>(src->data()->data()) + srcRect.y * src->m_rowLength + srcRect.x * PixelSize(src->format());

    // blit line by line
    for (s32 y = 0; y < srcRect.height; ++y)
    {
      // do blit
      scanline(dstLine, srcLine, dstRect.width);

      // move to next line
      dstLine = reinterpret_cast<u8*>(dstLine) + dst->m_rowLength;
      srcLine = reinterpret_cast<u8*>(srcLine) + src->m_rowLength;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImageUtils::ScanLineBltRGBA8888ToRGBA8888(void* dst, const void* src, s32 length)
{
  EGE_MEMCPY(dst, src, length * 4);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImageUtils::ScanLineBltRGBA8888ToRGB888(void* dst, const void* src, s32 length)
{
  u8* dest = reinterpret_cast<u8*>(dst);
  const u8* sorc = reinterpret_cast<const u8*>(src);

  while (0 != length)
  {
    *dest++ = *sorc++;
    *dest++ = *sorc++;
    *dest++ = *sorc++;

    ++sorc;

    --length;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImageUtils::FillLineBltRGBA8888(void* dst, u32 color, s32 length)
{
  u32* dest = reinterpret_cast<u32*>(dst);

  while (0 != length)
  {
    *dest++ = color;

    --length;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImageUtils::Fill(PImage& dst, const Recti& dstRect, const Color& color)
{
  u32 nativeColor = 0;

  PFNFILLLINEBLTFUNC filline = NULL;
  switch (dst->format())
  {
    case PF_RGBA_8888: 
      
      filline = FillLineBltRGBA8888; 
      nativeColor = color.packed();
      break;

    default:

      EGE_ASSERT(false && "Unsupported!");
      break;
  }

  if (filline)
  {
    // get first line beginings in destination and source buffers
    void* dstLine = reinterpret_cast<u8*>(dst->data()->data()) + dstRect.y * dst->m_rowLength + dstRect.x * PixelSize(dst->format());
    
    // fill line by line
    for (s32 y = 0; y < dstRect.height; ++y)
    {
      // do blit
      filline(dstLine, nativeColor, dstRect.width);

      // move to next line
      dstLine = reinterpret_cast<u8*>(dstLine) + dst->m_rowLength;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PImage ImageUtils::CreateImage(s32 width, s32 height, PixelFormat format, bool premultiplied, s32 rowLength, const PDataBuffer& data)
{
  // allocate image
  PImage image = ege_new Image(NULL);
  if (NULL != image)
  {
    image->m_width          = width;
    image->m_height         = height;
    image->m_format         = format;
    image->m_data           = data;
    image->m_rowLength      = rowLength;
    image->m_premultiplied  = premultiplied;
  }

  return image;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END