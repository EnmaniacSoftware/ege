#ifndef RESOURCELIBRARY_IMAGECOMPRESSIONFORMATS_H
#define RESOURCELIBRARY_IMAGECOMPRESSIONFORMATS_H

#include <QList>
#include <QString>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Available image compression formats. */
enum ImageCompressionFormat
{
  EImageCompressionPNG,
  EImageCompressionPVRTC_4bit,
  EImageCompressionPVRTC_2bit
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Image compression format info structure. */
struct ImageCompressionFormatInfo
{
  ImageCompressionFormatInfo(const QString& formatName, const QString& formatDisplayName, ImageCompressionFormat compressionFormat);

  ImageCompressionFormat  format;            /*!< Format value. */
  QString                 displayName;       /*!< Display name. */
  QString                 name;              /*!< Common use name (locale agnostic). */
};

typedef QList<ImageCompressionFormatInfo> ImageCompressionFormatInfoList;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Function returning list of supported image compression formats. */
extern ImageCompressionFormatInfoList SupportedImageCompressionFormats();
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCELIBRARY_IMAGECOMPRESSIONFORMATS_H
