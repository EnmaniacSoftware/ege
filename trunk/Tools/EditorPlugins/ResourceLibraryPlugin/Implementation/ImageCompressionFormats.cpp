#include "ImageCompressionFormats.h"
#include <QObject>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImageCompressionFormatInfo::ImageCompressionFormatInfo(const QString& formatName, const QString& formatDisplayName, ImageCompressionFormat compressionFormat)
  : format(compressionFormat)
  , displayName(formatDisplayName)
  , name(formatName)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImageCompressionFormatInfoList SupportedImageCompressionFormats()
{
  ImageCompressionFormatInfoList list;

  list << ImageCompressionFormatInfo("png",         QObject::tr("PNG"),         EImageCompressionPNG);
  list << ImageCompressionFormatInfo("pvrtc-4bit",  QObject::tr("PVRTC 4-bit"), EImageCompressionPVRTC_4bit);
  list << ImageCompressionFormatInfo("pvrtc-2bit",  QObject::tr("PVRTC 2-bit"), EImageCompressionPVRTC_2bit);

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
