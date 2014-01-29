#include "ImageFormats.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImageFormatInfo::ImageFormatInfo(const QString& formatName, const QString& formatDisplayName, QImage::Format imageFormat) : format(imageFormat)
                                                                                                                          , displayName(formatDisplayName)
                                                                                                                          , name(formatName)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImageFormatInfoList SupportedImageFormats()
{
  ImageFormatInfoList list;

  list << ImageFormatInfo("indexed",                QObject::tr("Indexed 8-bit"),                     QImage::Format_Indexed8);
  list << ImageFormatInfo("rgb",                    QObject::tr("32-bit RGB"),                        QImage::Format_RGB32);
  list << ImageFormatInfo("argb",                   QObject::tr("32-bit ARGB"),                       QImage::Format_ARGB32);
  list << ImageFormatInfo("argb-premultiplied",     QObject::tr("Premultiplied 32-bit ARGB"),         QImage::Format_ARGB32_Premultiplied);
  list << ImageFormatInfo("rgb565",                 QObject::tr("16-bit RGB (565)"),                  QImage::Format_RGB16);
  list << ImageFormatInfo("argb8565-premultiplied", QObject::tr("Premultiplied 24-bit ARGB (8565)"),  QImage::Format_ARGB8565_Premultiplied);
  list << ImageFormatInfo("rgb555",                 QObject::tr("15-bit RGB (555)"),                  QImage::Format_RGB555);

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
