#include "ImageFormats.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImageFormatInfo::ImageFormatInfo(QImage::Format imageFormat, const QString& imageDisplayName) : format(imageFormat),
                                                                                                displayName(imageDisplayName)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImageFormatInfoList SupportedImageFormats()
{
  ImageFormatInfoList list;

  list << ImageFormatInfo(QImage::Format_Indexed8, QObject::tr("Indexed 8-bit"));
  list << ImageFormatInfo(QImage::Format_RGB32, QObject::tr("32-bit RGB"));
  list << ImageFormatInfo(QImage::Format_ARGB32, QObject::tr("32-bit ARGB"));
  list << ImageFormatInfo(QImage::Format_ARGB32_Premultiplied, QObject::tr("Premultiplied 32-bit ARGB"));
  list << ImageFormatInfo(QImage::Format_RGB16, QObject::tr("16-bit RGB (565)"));
  list << ImageFormatInfo(QImage::Format_ARGB8565_Premultiplied, QObject::tr("Premultiplied 24-bit ARGB (8565)"));
  list << ImageFormatInfo(QImage::Format_RGB555, QObject::tr("15-bit RGB (555)"));

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
